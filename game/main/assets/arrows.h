#pragma once

#include "../playfield.h"

#include "amiga/bitmap.h"
#include "game/map.h"

using ArrowBitmap =
    MaskedInterleavedBitmap<tileWidth, tileHeight, playfieldDepth>;

enum class ArrowType : u8 {
  horizontal,
  vertical,
  pointEast,
  pointWest,
  pointSouth,
  pointNorth,
  northToEast,
  eastToSouth,
  southToWest,
  westToNorth,
  count,
};

struct ArrowBitmaps {
  ArrowBitmap bitmaps[static_cast<u16>(ArrowType::count)];
};
