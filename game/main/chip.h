#pragma once

#include "assets/mission/arrows.h"
#include "assets/mission/attack/bg.h"
#include "assets/mission/attack/units.h"
#include "assets/mission/menu_bitmaps.h"
#include "assets/mission/tileset.h"
#include "assets/mission/unit_bitmaps.h"
#include "assets/small_font.h"
#include "assets/sprite_images.h"
#include "frame_chip.h"

struct MissionAttackChip {
  AttackBackgrounds backgrounds;
  AttackUnitBitmaps units;
};

struct MissionChip {
  ArrowBitmaps arrows;
  Tileset tileset;
  UnitBitmaps units;
  MenuBitmaps menu;
  MissionAttackChip attack;
};

struct Chip {
  FrameChip frames[2];
  u16 zeroes[2]; // Dummy sprite
  MousePointerSpriteImage mousePointer;
  SmallFont smallFont;
  MissionChip mission;
};

extern Chip chip;

void initChip();
