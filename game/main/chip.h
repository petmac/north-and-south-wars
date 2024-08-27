#pragma once

#include "assets/mission/arrows.h"
#include "assets/mission/menu_bitmaps.h"
#include "assets/mission/tileset.h"
#include "assets/mission/unit_bitmaps.h"
#include "assets/small_font.h"
#include "assets/sprite_images.h"
#include "frame_chip.h"

struct Chip {
  FrameChip frames[2];
  u16 zeroes[2]; // Dummy sprite
  MousePointerSpriteImage mousePointer;
  SmallFont smallFont;
  ArrowBitmaps arrows;
  Tileset tileset;
  UnitBitmaps units;
  MenuBitmaps menu;
};

extern Chip chip;

void initChip();
