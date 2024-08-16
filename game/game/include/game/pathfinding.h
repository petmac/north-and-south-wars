#pragma once

#include "map.h" // TileCoords

using CameFrom = TileCoords[maxMapHeight][maxMapWidth];
using Cost = u16;
using CostSoFar = Cost[maxMapHeight][maxMapWidth];

struct PriorityQueue {
  static constexpr u16 capacity = 20;

  struct Item {
    TileCoords location;
    Cost cost;
  };

  u16 count;
  Item items[capacity];
};

struct Pathfinding {
  CameFrom cameFrom;
  CostSoFar costSoFar;
  PriorityQueue frontier;
  TileCoords start;
  TileCoords end;
};
