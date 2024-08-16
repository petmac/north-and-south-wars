#pragma once

#include "map.h" // TileCoords

constexpr u16 maxPathLength = 10;

using CameFrom = TileCoords[maxMapHeight][maxMapWidth];
using Cost = u16;
using CostSoFar = Cost[maxMapHeight][maxMapWidth];

struct Pathfinding {
  CameFrom cameFrom;
  CostSoFar costSoFar;
  TileCoords path[maxPathLength];
  u16 pathLength;
};
