#pragma once

#include "frame_chip.h"
#include "small_font.h"
#include "tileset.h"

struct Chip {
  FrameChip frames[2];
  SmallFont smallFont;
  Tileset tileset;
};

extern Chip chip;

void initChip();
