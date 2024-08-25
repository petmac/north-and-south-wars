#pragma once

#include "assets/arrows.h"
#include "assets/menu_bitmaps.h"
#include "assets/small_font.h"
#include "assets/sprite_images.h"
#include "assets/tileset.h"
#include "assets/unit_bitmaps.h"
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
