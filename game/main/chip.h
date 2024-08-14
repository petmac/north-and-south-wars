#pragma once

#include "assets/small_font.h"
#include "assets/sprite_images.h"
#include "assets/tileset.h"
#include "assets/units.h"
#include "frame_chip.h"

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
