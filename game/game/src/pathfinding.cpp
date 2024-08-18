#include "pathfinding.h"

#include "game/callbacks.h" // KPrintF, TODO Remove
#include "game/pathfinding.h"

// Algorithm based on:
// https://www.redblobgames.com/pathfinding/a-star/introduction.html
// https://www.redblobgames.com/pathfinding/a-star/implementation.html

static void insert(Frontier &frontier, TileCoords coords) {
  // Is the location is already in the queue?
  for (u16 existingLocationIndex = 0; existingLocationIndex < frontier.count;
       ++existingLocationIndex) {
    const TileCoords &existingLocation =
        frontier.locations[existingLocationIndex];
    if (existingLocation == coords) {
      return;
    }
  }

  // Is there enough space?
  if (frontier.count >= Frontier::capacity) {
    KPrintF("Frontier is full");
    return;
  }

  // Add the location
  frontier.locations[frontier.count++] = coords;
}

static TileCoords pop(Frontier &frontier) {
  return frontier.locations[--frontier.count];
}

static void considerNeighbour(Pathfinding &pathfinding, TileCoords current,
                              u16 nextColumn, u16 nextRow, Cost costToCurrent,
                              Cost unitMovementPoints) {
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

  // Not yet visited the next location?
  const TileCoords next = {
      .column = static_cast<u8>(nextColumn),
      .row = static_cast<u8>(nextRow),
  };
  if (existingCost == maxCost) {
    pathfinding.reachable.locations[pathfinding.reachable.count++] = next;
  }

  // Store the new lower cost
  existingCost = costToNext;

  // Add next to frontier
  insert(pathfinding.frontier, next);

  // Fix up the path with the lower cost route
  pathfinding.cameFrom[nextRow][nextColumn] = current;
}

void findPaths(Pathfinding &pathfinding, const Map &map, TileCoords start,
               u16 unitMovementPoints) {
  // Clear data structure
  // TODO No need to clear the whole map, just the area around the start
  for (u16 rowIndex = 0; rowIndex < maxMapHeight; ++rowIndex) {
    Cost(&row)[maxMapWidth] = pathfinding.costSoFar[rowIndex];
    for (u16 columnIndex = 0; columnIndex < maxMapWidth; ++columnIndex) {
      row[columnIndex] = maxCost;
    }
  }
  pathfinding.cameFrom[start.row][start.column] = start;
  pathfinding.costSoFar[start.row][start.column] = 0;
  pathfinding.frontier.count = 0;
  insert(pathfinding.frontier, start);
  pathfinding.reachable.count = 0;

  while (pathfinding.frontier.count > 0) {
    // What's the current best location to explore?
    const TileCoords current = pop(pathfinding.frontier);

    // Can we move to a neighbour?
    const Cost costToCurrent =
        pathfinding.costSoFar[current.row][current.column];

    // Where can we move to?
    if (current.row > 0) {
      considerNeighbour(pathfinding, current, current.column, current.row - 1,
                        costToCurrent, unitMovementPoints);
    }
    if (current.column < (map.width - 1)) {
      considerNeighbour(pathfinding, current, current.column + 1, current.row,
                        costToCurrent, unitMovementPoints);
    }
    if (current.row < (map.height - 1)) {
      considerNeighbour(pathfinding, current, current.column, current.row + 1,
                        costToCurrent, unitMovementPoints);
    }
    if (current.column > 0) {
      considerNeighbour(pathfinding, current, current.column - 1, current.row,
                        costToCurrent, unitMovementPoints);
    }
  }
}
