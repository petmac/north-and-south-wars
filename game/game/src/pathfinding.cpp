#include "pathfinding.h"

#include "game/callbacks.h" // KPrintF, TODO Remove
#include "game/pathfinding.h"

static void push(Frontier &frontier, TileCoords coords, Cost priority) {
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

static TileCoords pop(Frontier &frontier) {
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

static void considerNeighbour(Pathfinding &pathfinding, TileCoords current,
                              u16 nextColumn, u16 nextRow, TileCoords goal,
                              Cost costToCurrent, Cost unitMovementPoints) {
  // Does the unit have enough movement points to move to this location?
  // TODO Take unit and terrain into account
  const Cost costToEnterNext = 1;
  const Cost costToNext = costToCurrent + costToEnterNext;
  if (costToNext > unitMovementPoints) {
    return;
  }

  // Is the new route to next more expensive than an existing route?
  Cost &existingCost = pathfinding.costSoFar[nextRow][nextColumn];
  if (existingCost <= costToNext) {
    // Don't visit this neighbour again
    return;
  }

  // Store the new lower cost
  existingCost = costToNext;

  // Add next to frontier with heuristic to goal
  const TileCoords next = {
      .column = static_cast<u8>(nextColumn),
      .row = static_cast<u8>(nextRow),
  };
  const Cost priority = costToNext + heuristic(next, goal);
  push(pathfinding.frontier, next, priority);

  // Fix up the path with the lower cost route
  pathfinding.cameFrom[nextRow][nextColumn] = current;
}

static void aStarSearch(Pathfinding &pathfinding, const Map &map,
                        TileCoords start, TileCoords goal,
                        Cost unitMovementPoints) {
  push(pathfinding.frontier, start, 0);

  pathfinding.cameFrom[start.row][start.column] = start;
  pathfinding.costSoFar[start.row][start.column] = 0;

  while (pathfinding.frontier.count > 0) {
    // What's the current best location to explore?
    const TileCoords current = pop(pathfinding.frontier);

    // Found the goal?
    if (current == goal) {
      pathfinding.end = current;
      break;
    }

    // Can we move to a neighbour?
    const Cost costToCurrent =
        pathfinding.costSoFar[current.row][current.column];

    // Where can we move to?
    if (current.row > 0) {
      considerNeighbour(pathfinding, current, current.column, current.row - 1,
                        goal, costToCurrent, unitMovementPoints);
    }
    if (current.column < (map.width - 1)) {
      considerNeighbour(pathfinding, current, current.column + 1, current.row,
                        goal, costToCurrent, unitMovementPoints);
    }
    if (current.row < (map.height - 1)) {
      considerNeighbour(pathfinding, current, current.column, current.row + 1,
                        goal, costToCurrent, unitMovementPoints);
    }
    if (current.column > 0) {
      considerNeighbour(pathfinding, current, current.column - 1, current.row,
                        goal, costToCurrent, unitMovementPoints);
    }
  }
}

void findPath(Pathfinding &pathfinding, const Map &map, TileCoords start,
              TileCoords goal, u16 unitMovementPoints) {
  // Clear data structure
  for (u16 rowIndex = 0; rowIndex < maxMapHeight; ++rowIndex) {
    Cost(&row)[maxMapWidth] = pathfinding.costSoFar[rowIndex];
    for (u16 columnIndex = 0; columnIndex < maxMapWidth; ++columnIndex) {
      row[columnIndex] = ~0;
    }
  }
  pathfinding.frontier.count = 0;
  pathfinding.start = start;
  pathfinding.end = start; // Default to not finding the goal

  // https://www.redblobgames.com/pathfinding/a-star/introduction.html
  // https://www.redblobgames.com/pathfinding/a-star/implementation.html
  aStarSearch(pathfinding, map, start, goal, unitMovementPoints);
}
