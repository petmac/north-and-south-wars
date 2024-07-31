#pragma once

#include "playfield.h"

#include "amiga/bitmap.h"
#include "game/forces.h"
#include "game/units.h"

using UnitBitmap =
    MaskedInterleavedBitmap<unitWidth, unitHeight, playfieldDepth>;

struct ForceBitmaps {
  UnitBitmap units[static_cast<u16>(UnitType::count)];
};

struct UnitBitmaps {
  ForceBitmaps forces[static_cast<u16>(Force::count)];
};
