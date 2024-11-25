#pragma once

struct Map;
struct EnemyPathfinding;
struct TileCoords;
struct UnitDef;

void findPath(EnemyPathfinding &pathfinding, const Map &map,
              const TileCoords &start, const TileCoords &goal,
              const UnitDef &unitDef);
