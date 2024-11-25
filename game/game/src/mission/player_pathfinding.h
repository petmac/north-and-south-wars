#pragma once

struct Map;
struct PlayerPathfinding;
struct TileCoords;
struct UnitDef;

void findPaths(PlayerPathfinding &pathfinding, const Map &map,
               const TileCoords &start, const UnitDef &unitDef);
