#pragma once

struct Map;
struct Pathfinding;
struct TileCoords;

void findPath(Pathfinding &pathfinding, const Map &map, TileCoords start,
              TileCoords goal);
