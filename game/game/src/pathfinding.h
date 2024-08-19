#pragma once

#include "util/types.h"

struct Map;
struct Pathfinding;
struct TileCoords;

void findPaths(Pathfinding &pathfinding, const Map &map,
               const TileCoords &start, u16 unitMovementPoints);
