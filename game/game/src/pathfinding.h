#pragma once

struct Pathfinding;
struct TileCoords;

void findPath(Pathfinding &pathfinding, const TileCoords &from,
              const TileCoords &to);
