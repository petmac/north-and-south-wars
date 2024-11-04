#pragma once

#include "assets/campaign/campaign_bitmap.h"
#include "assets/mission/arrows.h"
#include "assets/mission/encounter/bg.h"
#include "assets/mission/encounter/units.h"
#include "assets/mission/menu_bitmaps.h"
#include "assets/mission/tileset.h"
#include "assets/mission/unit_bitmaps.h"
#include "assets/small_font.h"
#include "assets/sprite_images.h"
#include "assets/title/title_bitmap.h"
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
  Wave<5025> footsteps;
  Wave<1085> zoomIn;
  Wave<2630> zoomOut;
};

struct MainChip {
  MousePointerSpriteImage mousePointer;
  SmallFont smallFont;
  Wave<2181> cancel;
  Wave<813> ok;
};

struct TitleChip {
  TitleBitmap bitmap;
};

struct CampaignChip {
  CampaignBitmap bitmap;
};

struct Chip {
  FrameChip frames[2];
  u16 zeroes[2]; // Dummy sprite
  MainChip main;
  union {
    TitleChip title;
    CampaignChip campaign;
    MissionChip mission;
  };
};

extern Chip chip;

void initChip();
