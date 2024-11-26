#include "enemy_pathfinding.h"

#include "game/callbacks.h" // KPrintF, TODO Remove
#include "game/mission/enemy_pathfinding.h"
#include "game/mission/movement_cost.h" // calculateMovementCost
#include "game/mission/terrain.h"       // Terrain
#include "game/mission/tile.h"          // TileIndex
#include "game/mission/unit_defs.h"     // UnitDef

// Algorithm based on:
// https://www.redblobgames.com/pathfinding/a-star/introduction.html
// https://www.redblobgames.com/pathfinding/a-star/implementation.html

static void insert(PriorityQueue &frontier, const TileCoords &coords,
                   Cost priority) {
  // Is the location is already in the queue?
  for (u16 existingLocationIndex = 0; existingLocationIndex < frontier.count;
       ++existingLocationIndex) {
    // Skip if the location is different
    PriorityQueue::Location &existingLocation =
        frontier.locations[existingLocationIndex];
    if (existingLocation.coords != coords) {
      continue;
    }

    // Stop if the existing priority is lower or equal
    if (existingLocation.priority <= priority) {
      return;
    }

    // Replace cost if the new priority is lower
    existingLocation.priority = priority;
    return;
  }

  // Is there enough space?
  if (frontier.count >= PriorityQueue::capacity) {
    KPrintF("Bug: Priority queue is full");
    return;
  }

  // Add the location
  frontier.locations[frontier.count++] = (PriorityQueue::Location){
      .priority = priority,
      .coords = coords,
  };
}

static TileCoords pop(PriorityQueue &frontier) {
  // Find location with best (lowest) priority
  u16 bestIndex = 0;
  for (u16 locationIndex = 1; locationIndex < frontier.count; ++locationIndex) {
    if (frontier.locations[locationIndex].priority >=
        frontier.locations[bestIndex].priority) {
      continue;
    }

    bestIndex = locationIndex;
  }

  // Keep the best coords to return it later
  PriorityQueue::Location *const bestLocation = &frontier.locations[bestIndex];
  const TileCoords coords = bestLocation->coords;

  // Move the last location in the array over the best location
  *bestLocation = frontier.locations[--frontier.count];

  return coords;
}

static inline Cost manhattanDistance(const TileCoords &a, const TileCoords &b) {
  const u16 horizontalDistance =
      a.column < b.column ? b.column - a.column : a.column - b.column;
  const u16 verticalDistance = a.row < b.row ? b.row - a.row : a.row - b.row;

  return horizontalDistance + verticalDistance;
}

static void considerNeighbour(EnemyPathfinding &pathfinding, const Map &map,
                              const TileCoords &current, u16 nextColumn,
                              u16 nextRow, const TileCoords &goal,
                              Cost costToCurrent, const UnitDef &unitDef) {
  // Skip impassable tiles
  const TileIndex tileIndex = map.tiles[nextRow][nextColumn];
  const Terrain terrain = tileTerrain(tileIndex);
  const Cost movementCost =
      calculateMovementCost(terrain, unitDef.movementType);
  if (movementCost == maxCost) {
    return;
  }

  // Does the unit have enough movement points to move to this location?
  const Cost costToNext = costToCurrent + movementCost;
  if (costToNext > unitDef.movementPoints) {
    return;
  }

  // Is the new route to next more expensive than an existing route?
  Cost *const existingCost = &pathfinding.costSoFar[nextRow][nextColumn];
  if (*existingCost <= costToNext) {
    // Don't visit this neighbour again
    return;
  }

  // Skip tiles with living units on
  const TileCoords next = {
      .column = static_cast<u8>(nextColumn),
      .row = static_cast<u8>(nextRow),
  };
  for (u16 unitIndex = 0; unitIndex < map.unitCount; ++unitIndex) {
    const MapUnit &unit = map.units[unitIndex];

    // Dead units can be passed through
    if (unit.health == 0) {
      continue;
    }

    // If a unit is at this coordinate, stop considering it
    if (next == unit.coords) {
      return;
    }
  }

  // Store the new lower cost
  *existingCost = costToNext;

  // Add next to frontier
  const Cost priority = costToNext + manhattanDistance(next, goal);
  insert(pathfinding.frontier, next, priority);

  // Fix up the path with the lower cost route
  pathfinding.cameFrom[nextRow][nextColumn] = current;
}

void findPath(EnemyPathfinding &pathfinding, const Map &map,
              const TileCoords &start, const TileCoords &goal,
              const UnitDef &unitDef) {
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
  insert(pathfinding.frontier, start, 0);
  pathfinding.end = start;
  Cost endHeuristic = manhattanDistance(start, goal);

  while (pathfinding.frontier.count > 0) {
    // What's the current best location to explore?
    const TileCoords current = pop(pathfinding.frontier);

    // Is this the new end location?
    const Cost currentHeuristic = manhattanDistance(current, goal);
    if (currentHeuristic < endHeuristic) {
      pathfinding.end = current;
      endHeuristic = currentHeuristic;
    }

    // Found the goal?
    if (current == goal) {
      break;
    }

    // Can we move to a neighbour?
    const Cost costToCurrent =
        pathfinding.costSoFar[current.row][current.column];

    // Where can we move to?
    if (current.row > 0) {
      considerNeighbour(pathfinding, map, current, current.column,
                        current.row - 1, goal, costToCurrent, unitDef);
    }
    if (current.column < (map.width - 1)) {
      considerNeighbour(pathfinding, map, current, current.column + 1,
                        current.row, goal, costToCurrent, unitDef);
    }
    if (current.row < (map.height - 1)) {
      considerNeighbour(pathfinding, map, current, current.column,
                        current.row + 1, goal, costToCurrent, unitDef);
    }
    if (current.column > 0) {
      considerNeighbour(pathfinding, map, current, current.column - 1,
                        current.row, goal, costToCurrent, unitDef);
    }
  }
}
