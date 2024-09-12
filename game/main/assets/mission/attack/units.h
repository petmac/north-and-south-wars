#pragma once

#include "../../../playfield.h"

#include "amiga/bitmap.h"
#include "game/mission/forces.h"
#include "game/mission/unit_types.h"

constexpr u16 attackUnitWidth = 32;
constexpr u16 attackUnitHeight = 32;

using AttackUnitBitmap =
    MaskedInterleavedBitmap<attackUnitWidth, attackUnitHeight, playfieldDepth>;

struct AttackForceBitmaps {
  AttackUnitBitmap units[static_cast<u16>(UnitType::count)];
};

struct AttackUnitBitmaps {
  AttackForceBitmaps forces[static_cast<u16>(Force::count)];
};
