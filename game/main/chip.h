#pragma once

#include "frame_chip.h"
#include "small_font.h"

struct Chip {
  FrameChip frames[2];
  SmallFont smallFont;
};

extern Chip chip;

void initChip();
