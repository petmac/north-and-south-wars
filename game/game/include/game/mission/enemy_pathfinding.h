#pragma once

#include "map.h" // maxMapHeight, etc.

using CameFrom = TileCoords[maxMapHeight][maxMapWidth];
using Cost = u16;
using CostSoFar = Cost[maxMapHeight][maxMapWidth];

struct PriorityQueue {
  static constexpr u16 capacity = 6 * 6 * 2;

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
