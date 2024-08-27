#pragma once

#include "../../playfield.h"

#include "amiga/bitmap.h"

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
  reachable,
  attackable,
  count,
};

struct ArrowBitmaps {
  ArrowBitmap bitmaps[static_cast<u16>(ArrowType::count)];
};
