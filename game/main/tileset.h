#pragma once

#include "playfield.h"

#include "amiga/bitmap.h"
#include "game/map.h"

constexpr u16 tileCount = 256;

using TileBitmap = InterleavedBitmap<tileWidth, tileHeight, playfieldDepth>;

struct Tileset {
  TileBitmap tiles[tileCount];
};
