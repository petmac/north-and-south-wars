#pragma once

#include "map.h" // TileCoords

using CameFrom = TileCoords[maxMapHeight][maxMapWidth];
using Cost = u16;
using CostSoFar = Cost[maxMapHeight][maxMapWidth];

struct Frontier {
  static constexpr u16 capacity = maxMapWidth + maxMapHeight;

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
  Frontier frontier;
  TileCoords start;
  TileCoords end;
};
