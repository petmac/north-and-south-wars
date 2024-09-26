#pragma once

#include "../../../playfield.h"

#include "amiga/bitmap.h"
#include "game/mission/forces.h"
#include "game/mission/unit_types.h"

constexpr u16 encounterUnitWidth = 32;
constexpr u16 encounterUnitHeight = 32;

using EncounterUnitBitmap =
    MaskedInterleavedBitmap<encounterUnitWidth, encounterUnitHeight,
                            playfieldDepth>;

struct EncounterForceBitmaps {
  EncounterUnitBitmap units[static_cast<u16>(UnitType::count)];
};

struct EncounterUnitBitmaps {
  EncounterForceBitmaps forces[static_cast<u16>(Force::count)];
};
