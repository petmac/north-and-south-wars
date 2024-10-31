#pragma once

#include "assets/mission/arrows.h"
#include "assets/mission/encounter/bg.h"
#include "assets/mission/encounter/units.h"
#include "assets/mission/menu_bitmaps.h"
#include "assets/mission/tileset.h"
#include "assets/mission/unit_bitmaps.h"
#include "assets/small_font.h"
#include "assets/sprite_images.h"
#include "assets/wave.h"
#include "frame_chip.h"

struct MissionEncounterChip {
  EncounterBackgrounds backgrounds;
  EncounterUnitBitmaps units;
};

struct MissionChip {
  ArrowBitmaps arrows;
  Tileset tileset;
  UnitBitmaps units;
  MenuBitmaps menu;
  MissionEncounterChip encounter;
};

struct Waves {
  Wave<2181> cancel;
  Wave<813> ok;
};

struct Chip {
  FrameChip frames[2];
  u16 zeroes[2]; // Dummy sprite
  MousePointerSpriteImage mousePointer;
  SmallFont smallFont;
  MissionChip mission;
  Waves waves;
};

extern Chip chip;

void initChip();
