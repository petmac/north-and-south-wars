#include "pathfinding.h"

#include "game/callbacks.h" // KPrintF, TODO Remove
#include "game/pathfinding.h"

using Cost = u16;
using Graph = Map;
using Location = TileCoords;

struct Neighbours {
  bool north;
  bool east;
  bool south;
  bool west;
};

static constexpr Neighbours findNeighbours(const Graph &graph,
                                           Location location) {
  Neighbours n{};

  // Is north possible?
  // TODO Handle water and bridge directions
  if (location.row > 0) {
    n.north = true;
  }

  // Is east possible?
  if (location.column < (graph.width - 1)) {
    n.east = true;
  }

  // Is south possible?
  if (location.row < (graph.height - 1)) {
    n.south = true;
  }

  // Is west possible?
  if (location.column > 0) {
    n.west = true;
  }

  return n;
}

static constexpr Cost cost(const Graph &, Location, Location) {
  // TODO Take terrain and unit into account
  return 1;
}

struct PriorityQueue {
  static constexpr u16 capacity = 20;

  struct Item {
    Location location;
    Cost cost;
  };

  u16 count;
  Item items[capacity];

  constexpr bool empty() const { return count == 0; }

  constexpr void put(Location location, Cost cost) {
    // Is the location is already in the queue?
    for (u16 existingItemIndex = 0; existingItemIndex < count;
         ++existingItemIndex) {
      Item &existingItem = items[existingItemIndex];
      if (existingItem.location != location) {
        continue;
      }
      if (cost < existingItem.cost) {
        existingItem.cost = cost;
      }
      return;
    }

    // Is there enough space?
    if (count >= capacity) {
      KPrintF("Out of space in priority queue");
      return;
    }

    // Add the item.
    const Item item = {
        .location = location,
        .cost = cost,
    };
    items[count++] = item;
  }

  constexpr Location get() {
    Cost lowestCost = items[0].cost;
    u16 bestLocationIndex = 0;

    for (u16 itemIndex = 1; itemIndex < count; ++itemIndex) {
      const Cost cost = items[itemIndex].cost;
      if (cost < lowestCost) {
        lowestCost = cost;
        bestLocationIndex = itemIndex;
      }
    }

    const Location bestLocation = items[bestLocationIndex].location;

    // Remove the best item by overwriting it with the last item and shrinking.
    --count;
    items[bestLocationIndex] = items[count];

    return bestLocation;
  }
};

template <typename Key, typename Value> struct Dict {
  static constexpr u16 capacity = 20;

  struct Item {
    Key key;
    Value value;
  };

  u16 count;
  Item items[capacity];

  constexpr void put(Key key, Value value) {
    // Is the key already in the dictionary?
    for (u16 itemIndex = 0; itemIndex < count; ++itemIndex) {
      Item &item = items[itemIndex];

      if (item.key == key) {
        item.value = value;
        return;
      }
    }

    // Is there enough space?
    if (count >= capacity) {
      KPrintF("Out of space in dictionary");
      return;
    }

    // Add the item.
    const Item item = {
        .key = key,
        .value = value,
    };
    items[count++] = item;
  }

  constexpr Value *find(Key key) {
    for (u16 itemIndex = 0; itemIndex < count; ++itemIndex) {
      Item &item = items[itemIndex];
      if (item.key == key) {
        return &item.value;
      }
    }

    return nullptr;
  }

  constexpr Value get(Key key) {
    const Value *const value = find(key);
    if (value == nullptr) {
      return Value{};
    }

    return *value;
  }
};

static constexpr Cost dist(u16 a, u16 b) {
  if (a < b) {
    return b - a;
  } else {
    return a - b;
  }
}

static constexpr Cost heuristic(const Location &a, const Location &b) {
  return dist(a.column, b.column) + dist(a.row, b.row);
}

static constexpr void addNeighbour(CameFrom &cameFrom,
                                   Dict<Location, Cost> &costSoFar,
                                   PriorityQueue &frontier, const Graph &graph,
                                   Location current, Location next,
                                   Location goal) {
  // Compute cost to reach next from current
  const Cost newCost = costSoFar.get(current) + cost(graph, current, next);

  // Have we already visited next?
  Cost *const existingCost = costSoFar.find(next);
  if (existingCost != nullptr) {
    // Already visited. Have we already visited with a lower cost?
    if (*existingCost <= newCost) {
      // Don't add this path.
      return;
    }

    // Found a better path
    *existingCost = newCost;
  } else {
    // This is a new path
    costSoFar.put(next, newCost);
  }

  // Add next to frontier with heuristic to goal
  const Cost priority = newCost + heuristic(next, goal);
  frontier.put(next, priority);

  // Fix up the path with the lower cost route
  cameFrom[next.row][next.column] = current;
}

static constexpr void aStarSearch(CameFrom &cameFrom,
                                  Dict<Location, Cost> &costSoFar,
                                  const Graph &graph, Location start,
                                  Location goal) {
  PriorityQueue frontier{};
  frontier.put(start, 0);

  cameFrom[start.row][start.column] = start;
  costSoFar.put(start, 0);

  while (!frontier.empty()) {
    const Location current = frontier.get();

    if (current == goal) {
      break;
    }

    const Neighbours neighbours = findNeighbours(graph, current);
    if (neighbours.north) {
      const Location next{
          .column = current.column,
          .row = static_cast<u8>(current.row - 1),
      };
      addNeighbour(cameFrom, costSoFar, frontier, graph, current, next, goal);
    }
    if (neighbours.east) {
      const Location next{
          .column = static_cast<u8>(current.column + 1),
          .row = current.row,
      };
      addNeighbour(cameFrom, costSoFar, frontier, graph, current, next, goal);
    }
    if (neighbours.south) {
      const Location next{
          .column = current.column,
          .row = static_cast<u8>(current.row + 1),
      };
      addNeighbour(cameFrom, costSoFar, frontier, graph, current, next, goal);
    }
    if (neighbours.west) {
      const Location next{
          .column = static_cast<u8>(current.column - 1),
          .row = current.row,
      };
      addNeighbour(cameFrom, costSoFar, frontier, graph, current, next, goal);
    }
  }
}

void findPath(Pathfinding &pathfinding, const Map &map, const TileCoords &start,
              const TileCoords &goal) {
  // https://www.redblobgames.com/pathfinding/a-star/introduction.html
  // https://www.redblobgames.com/pathfinding/a-star/implementation.html
  Dict<Location, Cost> costSoFar{};
  aStarSearch(pathfinding.cameFrom, costSoFar, map, start, goal);

  // Copy path to output
  TileCoords next = goal;
  pathfinding.pathLength = 0;
  while (pathfinding.pathLength < maxPathLength) {
    const Location prev = pathfinding.cameFrom[next.row][next.column];
    pathfinding.path[pathfinding.pathLength++] = next;
    if (prev == start) {
      break;
    }
    next = prev;
  }
}
