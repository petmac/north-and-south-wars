#pragma once

#include "../../../playfield.h"

#include "amiga/bitmap.h"

constexpr u16 attackBackgroundWidth = 160;
constexpr u16 attackBackgroundSliceHeight = 128;
constexpr u16 attackBackgroundSliceCount = 2;

using AttackBackgroundSlice =
    InterleavedBitmap<attackBackgroundWidth, attackBackgroundSliceHeight,
                      playfieldDepth>;

using AttackBackground = AttackBackgroundSlice[attackBackgroundSliceCount];

struct AttackBackgrounds {
  AttackBackground bridge;
  AttackBackground mountain;
  AttackBackground plain;
  AttackBackground road;
  AttackBackground woods;
};
