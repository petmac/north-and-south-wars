#include "pathfinding.h"

#include "game/callbacks.h" // KPrintF, TODO Remove
#include "game/pathfinding.h"

struct Neighbours {
  bool north;
  bool east;
  bool south;
  bool west;
};

static constexpr Neighbours findNeighbours(const Map &map, TileCoords coords) {
  Neighbours n{};

  // Is north possible?
  // TODO Handle water and bridge directions
  if (coords.row > 0) {
    n.north = true;
  }

  // Is east possible?
  if (coords.column < (map.width - 1)) {
    n.east = true;
  }

  // Is south possible?
  if (coords.row < (map.height - 1)) {
    n.south = true;
  }

  // Is west possible?
  if (coords.column > 0) {
    n.west = true;
  }

  return n;
}

constexpr void push(Frontier &frontier, TileCoords coords, Cost priority) {
  // Is the location is already in the queue?
  for (u16 existingLocationIndex = 0; existingLocationIndex < frontier.count;
       ++existingLocationIndex) {
    Frontier::Location &existingLocation =
        frontier.locations[existingLocationIndex];
    if (existingLocation.coords != coords) {
      continue;
    }
    if (priority < existingLocation.priority) {
      existingLocation.priority = priority;
    }
    return;
  }

  // Is there enough space?
  if (frontier.count >= Frontier::capacity) {
    KPrintF("Frontier is full");
    return;
  }

  // Add the location.
  const Frontier::Location location = {
      .coords = coords,
      .priority = priority,
  };
  frontier.locations[frontier.count++] = location;
}

constexpr TileCoords pop(Frontier &frontier) {
  Cost bestPriority = frontier.locations[0].priority;
  u16 bestLocationIndex = 0;

  for (u16 locationIndex = 1; locationIndex < frontier.count; ++locationIndex) {
    const Cost priority = frontier.locations[locationIndex].priority;
    if (priority < bestPriority) {
      bestPriority = priority;
      bestLocationIndex = locationIndex;
    }
  }

  const TileCoords bestLocation = frontier.locations[bestLocationIndex].coords;

  // Remove the best location by overwriting it with the last location and
  // shrinking.
  --frontier.count;
  frontier.locations[bestLocationIndex] = frontier.locations[frontier.count];

  return bestLocation;
}

static constexpr Cost heuristic(TileCoords a, TileCoords b) {
  const s16 y = static_cast<s16>(a.row) - static_cast<s16>(b.row);
  const s16 x = static_cast<s16>(a.column) - static_cast<s16>(b.column);

  return (y * y) + (x * x);
}

static constexpr void addNeighbour(Pathfinding &pathfinding, TileCoords current,
                                   TileCoords next, TileCoords goal) {
  // Compute cost to reach next from current
  // TODO Take unit and terrain into account
  const Cost newCost = pathfinding.costSoFar[current.row][current.column] + 1;

  // Is the new route to next more expensive than an existing route?
  Cost &existingCost = pathfinding.costSoFar[next.row][next.column];
  if (existingCost <= newCost) {
    // Don't visit this neighbour again
    return;
  }

  // Store the new lower cost
  existingCost = newCost;

  // Add next to frontier with heuristic to goal
  const Cost priority = newCost + heuristic(next, goal);
  push(pathfinding.frontier, next, priority);

  // Fix up the path with the lower cost route
  pathfinding.cameFrom[next.row][next.column] = current;
}

static constexpr void aStarSearch(Pathfinding &pathfinding, const Map &map,
                                  TileCoords start, TileCoords goal) {
  push(pathfinding.frontier, start, 0);

  pathfinding.cameFrom[start.row][start.column] = start;
  pathfinding.costSoFar[start.row][start.column] = 0;

  while (pathfinding.frontier.count > 0) {
    const TileCoords current = pop(pathfinding.frontier);

    if (current == goal) {
      break;
    }

    const Neighbours neighbours = findNeighbours(map, current);
    if (neighbours.north) {
      const TileCoords next{
          .column = current.column,
          .row = static_cast<u8>(current.row - 1),
      };
      addNeighbour(pathfinding, current, next, goal);
    }
    if (neighbours.east) {
      const TileCoords next{
          .column = static_cast<u8>(current.column + 1),
          .row = current.row,
      };
      addNeighbour(pathfinding, current, next, goal);
    }
    if (neighbours.south) {
      const TileCoords next{
          .column = current.column,
          .row = static_cast<u8>(current.row + 1),
      };
      addNeighbour(pathfinding, current, next, goal);
    }
    if (neighbours.west) {
      const TileCoords next{
          .column = static_cast<u8>(current.column - 1),
          .row = current.row,
      };
      addNeighbour(pathfinding, current, next, goal);
    }
  }
}

void findPath(Pathfinding &pathfinding, const Map &map, TileCoords start,
              TileCoords goal) {
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
  aStarSearch(pathfinding, map, start, goal);
}
