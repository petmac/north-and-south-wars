#pragma once

#include "forces.h"
#include "units.h"

constexpr u16 maxMapWidth = 30;
constexpr u16 maxMapHeight = 20;
constexpr u16 tileWidth = 16;
constexpr u16 tileHeight = 16;
constexpr u16 maxMapUnits = 32;

enum class TileIndex : u8;

struct TileCoords {
  u8 column;
  u8 row;
};

constexpr bool operator==(const TileCoords &a, const TileCoords &b) {
  return (a.column == b.column) && (a.row == b.row);
}

struct MapUnit {
  TileCoords coords;
  Force force;
  UnitType type;
};

struct Map {
  u8 width;
  u8 height;
  TileIndex tiles[maxMapHeight][maxMapWidth];
  u8 unitCount;
  MapUnit units[maxMapUnits];
};
