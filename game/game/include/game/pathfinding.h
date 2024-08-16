#pragma once

#include "map.h" // TileCoords

constexpr u16 maxPathLength = 10;

using CameFrom = TileCoords[maxMapHeight][maxMapWidth];

struct Pathfinding {
  CameFrom cameFrom;
  TileCoords path[maxPathLength];
  u16 pathLength;
};
