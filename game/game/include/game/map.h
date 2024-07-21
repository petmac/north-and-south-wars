#pragma once

#include "util/types.h"

constexpr u16 maxMapWidth = 30;
constexpr u16 maxMapHeight = 20;
constexpr u16 tileWidth = 16;
constexpr u16 tileHeight = 16;

struct Map {
  u8 width;
  u8 height;
  u8 tiles[maxMapHeight][maxMapWidth];
};
