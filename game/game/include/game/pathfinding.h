#pragma once

#include "map.h" // TileCoords

using CameFrom = TileCoords[maxMapHeight][maxMapWidth];
using Cost = u16;
using CostSoFar = Cost[maxMapHeight][maxMapWidth];

struct Pathfinding {
  CameFrom cameFrom;
  CostSoFar costSoFar;
  TileCoords start;
  TileCoords end;
};
