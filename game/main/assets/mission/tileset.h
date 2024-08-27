#pragma once

#include "../../playfield.h"

#include "amiga/bitmap.h"
#include "game/mission/tile.h"

using TileBitmap = InterleavedBitmap<tileWidth, tileHeight, playfieldDepth>;

struct Tileset {
  TileBitmap tiles[static_cast<u16>(TileIndex::count)];
};
