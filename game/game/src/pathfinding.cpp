#include "pathfinding.h"

#include "game/callbacks.h"     // KPrintF, TODO Remove
#include "game/movement_cost.h" // calculateMovementCost
#include "game/pathfinding.h"
#include "game/terrain.h"   // Terrain
#include "game/tile.h"      // TileIndex
#include "game/unit_defs.h" // UnitDef

// Algorithm based on:
// https://www.redblobgames.com/pathfinding/a-star/introduction.html
// https://www.redblobgames.com/pathfinding/a-star/implementation.html

static void insert(Frontier &frontier, const TileCoords &coords) {
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

static void considerNeighbour(Pathfinding &pathfinding, const Map &map,
                              const TileCoords &current, u16 nextColumn,
                              u16 nextRow, Cost costToCurrent,
                              const UnitDef &unitDef) {
  // Skip impassable tiles
  const TileIndex tileIndex = map.tiles[nextRow][nextColumn];
  const Terrain terrain = tileTerrain(tileIndex);
  const Cost movementCost =
      calculateMovementCost(terrain, unitDef.movementType);
  if (movementCost == maxCost) {
    return;
  }

  // Does the unit have enough movement points to move to this location?
  // TODO Take unit and terrain into account
  const Cost costToNext = costToCurrent + movementCost;
  if (costToNext > unitDef.movementPoints) {
    return;
  }

  // Is the new route to next more expensive than an existing route?
  Cost &existingCost = pathfinding.costSoFar[nextRow][nextColumn];
  if (existingCost <= costToNext) {
    // Don't visit this neighbour again
    return;
  }

  // Skip tiles with units on
  const TileCoords next = {
      .column = static_cast<u8>(nextColumn),
      .row = static_cast<u8>(nextRow),
  };
  for (u16 unitIndex = 0; unitIndex < map.unitCount; ++unitIndex) {
    const MapUnit &unit = map.units[unitIndex];
    if (next == unit.coords) {
      return;
    }
  }

  // Not yet visited the next location?
  if (existingCost == maxCost) {
    // Add to the list of reachable locations
    if (pathfinding.reachable.count < Frontier::capacity) {
      pathfinding.reachable.locations[pathfinding.reachable.count++] = next;
    }
  }

  // Store the new lower cost
  existingCost = costToNext;

  // Add next to frontier
  insert(pathfinding.frontier, next);

  // Fix up the path with the lower cost route
  pathfinding.cameFrom[nextRow][nextColumn] = current;
}

void findPaths(Pathfinding &pathfinding, const Map &map,
               const TileCoords &start, const UnitDef &unitDef) {
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
      considerNeighbour(pathfinding, map, current, current.column,
                        current.row - 1, costToCurrent, unitDef);
    }
    if (current.column < (map.width - 1)) {
      considerNeighbour(pathfinding, map, current, current.column + 1,
                        current.row, costToCurrent, unitDef);
    }
    if (current.row < (map.height - 1)) {
      considerNeighbour(pathfinding, map, current, current.column,
                        current.row + 1, costToCurrent, unitDef);
    }
    if (current.column > 0) {
      considerNeighbour(pathfinding, map, current, current.column - 1,
                        current.row, costToCurrent, unitDef);
    }
  }
}
