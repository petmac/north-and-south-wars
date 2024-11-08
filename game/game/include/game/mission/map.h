#pragma once

#include "tile_coords.h"

constexpr u16 maxMapWidth = 20;
constexpr u16 maxMapHeight = 16;
constexpr u16 tileWidth = 16;
constexpr u16 tileHeight = 16;
constexpr u16 maxMapUnits = 32;

enum class Force : u8;
enum class TileIndex : u8;
enum class UnitType : u8;

struct MapUnit {
  TileCoords coords;
  Force force;
  UnitType type;
  u8 health; // Dead when health is zero
  u8 moved;
};

struct Map {
  u8 width;
  u8 height;
  TileIndex tiles[maxMapHeight][maxMapWidth];
  u8 unitCount;
  MapUnit units[maxMapUnits];
};
