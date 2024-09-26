#pragma once

#include "../../../playfield.h"

#include "amiga/bitmap.h"

constexpr u16 encounterBackgroundWidth = 160;
constexpr u16 encounterBackgroundHeight = 256;
constexpr u16 encounterBackgroundSliceHeight = 32;
constexpr u16 encounterBackgroundSliceCount =
    encounterBackgroundHeight / encounterBackgroundSliceHeight;

using EncounterBackgroundSlice =
    InterleavedBitmap<encounterBackgroundWidth, encounterBackgroundSliceHeight,
                      playfieldDepth>;

using EncounterBackground =
    EncounterBackgroundSlice[encounterBackgroundSliceCount];

struct EncounterBackgrounds {
  EncounterBackground bridge;
  EncounterBackground mountain;
  EncounterBackground plain;
  EncounterBackground road;
  EncounterBackground woods;
};
