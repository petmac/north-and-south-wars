#pragma once

#include "util/types.h"

struct Map;
struct Pathfinding;
struct TileCoords;

void findPath(Pathfinding &pathfinding, const Map &map, TileCoords start,
              TileCoords goal, u16 unitMovementPoints);
