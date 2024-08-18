#pragma once

#include "map.h" // TileCoords

using CameFrom = TileCoords[maxMapHeight][maxMapWidth];
using Cost = u16;
using CostSoFar = Cost[maxMapHeight][maxMapWidth];

constexpr Cost maxCost = 65535;

struct Frontier {
  static constexpr u16 capacity =
      maxMapWidth +
      maxMapHeight; // TODO Should be computed using maximum movement distance

  u16 count;
  TileCoords locations[capacity];
};

struct Pathfinding {
  CameFrom cameFrom;
  CostSoFar costSoFar;
  Frontier frontier;
  Frontier reachable;
};
