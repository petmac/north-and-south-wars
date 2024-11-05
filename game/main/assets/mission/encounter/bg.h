#pragma once

#include "../../../playfield.h"

#include "amiga/bitmap.h"

constexpr u16 encounterBackgroundWidth = 160;
constexpr u16 encounterBackgroundHeight = 256;

using EncounterBackground =
    InterleavedBitmap<encounterBackgroundWidth, encounterBackgroundHeight,
                      playfieldDepth>;

struct EncounterBackgrounds {
  EncounterBackground bridge;
  EncounterBackground mountain;
  EncounterBackground plain;
  EncounterBackground road;
  EncounterBackground woods;
};
