#pragma once

struct Map;
struct Pathfinding;
struct TileCoords;
struct UnitDef;

void findPaths(Pathfinding &pathfinding, const Map &map,
               const TileCoords &start, const UnitDef &unitDef);
