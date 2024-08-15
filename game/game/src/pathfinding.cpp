#include "pathfinding.h"

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
    // TODO What to do if the location is already in the queue?

    // Is there enough space?
    if (count >= capacity) {
      // TODO Log warning
      for (;;) {
      }
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
      const Cost cost = items[bestLocationIndex].cost;
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
      // TODO Log warning
      for (;;) {
      }
      return;
    }

    // Add the item.
    const Item item = {
        .key = key,
        .value = value,
    };
    items[count++] = item;
  }

  constexpr bool contains(Key key) const {
    for (u16 itemIndex = 0; itemIndex < count; ++itemIndex) {
      if (items[itemIndex].key == key) {
        return true;
      }
    }

    return false;
  }

  constexpr Value get(Key key) const {
    for (u16 itemIndex = 0; itemIndex < count; ++itemIndex) {
      const Item &item = items[itemIndex];

      if (item.key == key) {
        return item.value;
      }
    }

    return Value{};
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

static constexpr void addNeighbour(Dict<Location, Location> &came_from,
                                   Dict<Location, Cost> &cost_so_far,
                                   PriorityQueue &frontier, const Graph &graph,
                                   Location current, Location next,
                                   Location goal) {
  const Cost new_cost = cost_so_far.get(current) + cost(graph, current, next);
  if (!cost_so_far.contains(next) || new_cost < cost_so_far.get(next)) {
    cost_so_far.put(next, new_cost);
    const Cost priority = new_cost + heuristic(next, goal);
    frontier.put(next, priority);
    came_from.put(next, current);
  }
}

static constexpr void aStarSearch(Dict<Location, Location> &came_from,
                                  Dict<Location, Cost> &cost_so_far,
                                  const Graph &graph, Location start,
                                  Location goal) {
  PriorityQueue frontier{};
  frontier.put(start, 0);

  came_from.put(start, start);
  cost_so_far.put(start, 0);

  while (!frontier.empty()) {
    Location current = frontier.get();

    if (current == goal) {
      break;
    }

    const Neighbours neighbours = findNeighbours(graph, current);
    if (neighbours.north) {
      const Location next{
          .column = current.column,
          .row = static_cast<u8>(current.row - 1),
      };
      addNeighbour(came_from, cost_so_far, frontier, graph, current, next,
                   goal);
    }
    if (neighbours.east) {
      const Location next{
          .column = static_cast<u8>(current.column + 1),
          .row = current.row,
      };
      addNeighbour(came_from, cost_so_far, frontier, graph, current, next,
                   goal);
    }
    if (neighbours.south) {
      const Location next{
          .column = current.column,
          .row = static_cast<u8>(current.row + 1),
      };
      addNeighbour(came_from, cost_so_far, frontier, graph, current, next,
                   goal);
    }
    if (neighbours.west) {
      const Location next{
          .column = static_cast<u8>(current.column - 1),
          .row = current.row,
      };
      addNeighbour(came_from, cost_so_far, frontier, graph, current, next,
                   goal);
    }
  }
}

void findPath(Pathfinding &pathfinding, const Map &map, const TileCoords &from,
              const TileCoords &to) {
  // https://www.redblobgames.com/pathfinding/a-star/introduction.html
  // https://www.redblobgames.com/pathfinding/a-star/implementation.html
  Dict<Location, Location> came_from{};
  Dict<Location, Cost> cost_so_far{};
  aStarSearch(came_from, cost_so_far, map, from, to);

  // Copy path to output
  TileCoords next = to;
  pathfinding.pathLength = 0;
  while (pathfinding.pathLength < maxPathLength) {
    if (!came_from.contains(next)) {
      break;
    }
    const TileCoords prev = came_from.get(next);
    pathfinding.path[pathfinding.pathLength++] = next;
    if (prev == from) {
      break;
    }
    next = prev;
  }
}
