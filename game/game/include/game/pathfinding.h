#pragma once

#include "map.h" // TileCoords

constexpr u16 maxPathLength = 10;

struct Pathfinding {
  TileCoords path[maxPathLength];
  u16 pathLength;
};
