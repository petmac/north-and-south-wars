#pragma once

#include "map.h" // maxMapHeight, etc.

using CameFrom = TileCoords[maxMapHeight][maxMapWidth];
using Cost = u16;
using CostSoFar = Cost[maxMapHeight][maxMapWidth];

struct Frontier {
  static constexpr u16 capacity = 6 * 6 * 2;

  u16 count;
  TileCoords locations[capacity];
};

struct PlayerPathfinding {
  CameFrom cameFrom;
  CostSoFar costSoFar;
  Frontier frontier;
  Frontier reachable;
};
