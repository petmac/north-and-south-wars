#pragma once

#include "frame_chip.h"

using SmallFont = MaskedInterleavedBitmap<128, 48, 5>;

struct Chip {
  FrameChip frames[2];
  SmallFont smallFont;
};

extern Chip chip;

void initChip();
