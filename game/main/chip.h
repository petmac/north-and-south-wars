#pragma once

#include "frame_chip.h"
#include "small_font.h"
#include "sprite_images.h"
#include "tileset.h"
#include "units.h"

struct Chip {
  FrameChip frames[2];
  u16 zeroes[2]; // Dummy sprite
  MousePointerSpriteImage mousePointer;
  SmallFont smallFont;
  Tileset tileset;
  UnitBitmaps units;
};

extern Chip chip;

void initChip();
