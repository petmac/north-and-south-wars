#include "pathfinding.h"

#include "game/callbacks.h" // KPrintF, TODO Remove
#include "game/pathfinding.h"

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

constexpr bool empty(const Frontier &frontier) { return frontier.count == 0; }

constexpr void put(Frontier &frontier, Location location, Cost cost) {
  // Is the location is already in the queue?
  for (u16 existingItemIndex = 0; existingItemIndex < frontier.count;
       ++existingItemIndex) {
    Frontier::Item &existingItem = frontier.items[existingItemIndex];
    if (existingItem.location != location) {
      continue;
    }
    if (cost < existingItem.cost) {
      existingItem.cost = cost;
    }
    return;
  }

  // Is there enough space?
  if (frontier.count >= Frontier::capacity) {
    KPrintF("Out of space in priority queue");
    return;
  }

  // Add the item.
  const Frontier::Item item = {
      .location = location,
      .cost = cost,
  };
  frontier.items[frontier.count++] = item;
}

constexpr Location get(Frontier &frontier) {
  Cost lowestCost = frontier.items[0].cost;
  u16 bestLocationIndex = 0;

  for (u16 itemIndex = 1; itemIndex < frontier.count; ++itemIndex) {
    const Cost cost = frontier.items[itemIndex].cost;
    if (cost < lowestCost) {
      lowestCost = cost;
      bestLocationIndex = itemIndex;
    }
  }

  const Location bestLocation = frontier.items[bestLocationIndex].location;

  // Remove the best item by overwriting it with the last item and shrinking.
  --frontier.count;
  frontier.items[bestLocationIndex] = frontier.items[frontier.count];

  return bestLocation;
}

static constexpr Cost heuristic(const Location &a, const Location &b) {
  const s16 y = static_cast<s16>(a.row) - static_cast<s16>(b.row);
  const s16 x = static_cast<s16>(a.column) - static_cast<s16>(b.column);

  return (y * y) + (x * x);
}

static constexpr void addNeighbour(CameFrom &cameFrom, CostSoFar &costSoFar,
                                   Frontier &frontier, const Graph &graph,
                                   Location current, Location next,
                                   Location goal) {
  // Compute cost to reach next from current
  const Cost newCost =
      costSoFar[current.row][current.column] + cost(graph, current, next);

  // Is the new route to next more expensive than an existing route?
  Cost &existingCost = costSoFar[next.row][next.column];
  if (existingCost <= newCost) {
    // Don't visit this neighbour again
    return;
  }

  // Store the new lower cost
  existingCost = newCost;

  // Add next to frontier with heuristic to goal
  const Cost priority = newCost + heuristic(next, goal);
  put(frontier, next, priority);

  // Fix up the path with the lower cost route
  cameFrom[next.row][next.column] = current;
}

static constexpr void aStarSearch(CameFrom &cameFrom, CostSoFar &costSoFar,
                                  Frontier &frontier, const Graph &graph,
                                  Location start, Location goal) {
  put(frontier, start, 0);

  cameFrom[start.row][start.column] = start;
  costSoFar[start.row][start.column] = 0;

  while (!empty(frontier)) {
    const Location current = get(frontier);

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
  // Clear data structure
  for (u16 rowIndex = 0; rowIndex < maxMapHeight; ++rowIndex) {
    Cost(&row)[maxMapWidth] = pathfinding.costSoFar[rowIndex];
    for (u16 columnIndex = 0; columnIndex < maxMapWidth; ++columnIndex) {
      row[columnIndex] = ~0;
    }
  }
  pathfinding.frontier.count = 0;
  pathfinding.start = start;
  pathfinding.end = goal; // TODO Replace with shorter path

  // https://www.redblobgames.com/pathfinding/a-star/introduction.html
  // https://www.redblobgames.com/pathfinding/a-star/implementation.html
  aStarSearch(pathfinding.cameFrom, pathfinding.costSoFar, pathfinding.frontier,
              map, start, goal);
}
