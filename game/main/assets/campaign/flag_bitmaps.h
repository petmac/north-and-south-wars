#pragma once

#include "../../playfield.h"

#include "amiga/bitmap.h"
#include "game/mission/forces.h"

constexpr u16 flagWidth = 16;
constexpr u16 flagHeight = 16;
constexpr u16 flagFrameCount = 3;

using FlagFrame =
    MaskedInterleavedBitmap<flagWidth, flagHeight, playfieldDepth>;

using FlagFrames = FlagFrame[flagFrameCount];

using FlagBitmaps = FlagFrames[static_cast<u16>(Force::count)];
