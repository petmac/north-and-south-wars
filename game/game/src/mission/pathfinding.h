#pragma once

struct Map;
struct Pathfinding;
struct TileCoords;
struct UnitDef;
struct UnitInstance;

void findPaths(Pathfinding &pathfinding, const Map &map,
               const UnitInstance units[], const TileCoords &start,
               const UnitDef &unitDef);
