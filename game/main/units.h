#pragma once

#include "playfield.h"

#include "amiga/bitmap.h"
#include "game/teams.h"
#include "game/units.h"

using UnitBitmap =
    MaskedInterleavedBitmap<unitWidth, unitHeight, playfieldDepth>;

struct TeamBitmaps {
  UnitBitmap units[static_cast<u16>(UnitType::count)];
};

struct UnitBitmaps {
  TeamBitmaps teams[static_cast<u16>(Team::count)];
};
