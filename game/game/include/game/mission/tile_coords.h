#pragma once

#include "util/types.h"

struct TileCoords {
  u8 column;
  u8 row;
};

constexpr bool operator==(const TileCoords &a, const TileCoords &b) {
  return (a.column == b.column) && (a.row == b.row);
}
