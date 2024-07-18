#pragma once

#include "playfield.h"

#include "amiga/bitmap.h"

constexpr u16 tileWidth = 16;
constexpr u16 tileHeight = 16;
constexpr u16 tileCount = 256;

using TileBitmap = InterleavedBitmap<tileWidth, tileHeight, playfieldDepth>;

struct Tileset {
  TileBitmap tiles[tileCount];
};
