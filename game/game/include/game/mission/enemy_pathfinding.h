#pragma once

#include "map.h" // maxMapHeight, etc.

using CameFrom = TileCoords[maxMapHeight][maxMapWidth];
using Cost = u16;
using CostSoFar = Cost[maxMapHeight][maxMapWidth];

struct PriorityQueue {
  static constexpr u16 capacity =
      maxMapWidth +
      maxMapHeight; // TODO Should be computed using maximum movement distance

  struct Location {
    Cost priority;
    TileCoords coords;
  };

  u16 count;
  Location locations[capacity];
};

struct EnemyPathfinding {
  CameFrom cameFrom;
  CostSoFar costSoFar;
  PriorityQueue frontier;
  TileCoords end;
};
