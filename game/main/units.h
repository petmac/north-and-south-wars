#pragma once

#include "playfield.h"

#include "amiga/bitmap.h"

using UnitBitmap = MaskedInterleavedBitmap<16, 16, playfieldDepth>;

enum class UnitType : u8 {
  infantry,
  mech,
  lightTank,
  heavyTank,
  count,
};

enum class Team : u8 {
  north,
  south,
  count,
};

struct TeamBitmaps {
  UnitBitmap units[static_cast<u16>(UnitType::count)];
};

struct UnitBitmaps {
  TeamBitmaps teams[static_cast<u16>(Team::count)];
};
