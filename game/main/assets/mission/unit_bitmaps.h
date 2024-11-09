#pragma once

#include "../../playfield.h"

#include "amiga/bitmap.h"
#include "game/mission/forces.h"
#include "game/mission/unit_types.h"

constexpr u16 unitWidth = 32;
constexpr u16 unitHeight = 16;

using UnitBitmap =
    MaskedInterleavedBitmap<unitWidth, unitHeight, playfieldDepth>;

struct ForceBitmaps {
  UnitBitmap units[static_cast<u16>(UnitType::count)];
};

struct UnitBitmaps {
  ForceBitmaps forces[static_cast<u16>(Force::count)];
};
