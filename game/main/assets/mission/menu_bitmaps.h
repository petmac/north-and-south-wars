#pragma once

#include "../../playfield.h"

#include "amiga/bitmap.h"
#include "game/menu.h"

enum class MenuButtonBitmapIndex : u8 {
  attack,
  wait,
  endTurn,
  count,
};

using MenuButtonBitmap =
    MaskedInterleavedBitmap<menuButtonWidth, menuButtonHeight, playfieldDepth>;

struct MenuBitmaps {
  MenuButtonBitmap buttons[static_cast<u16>(MenuButtonBitmapIndex::count)];
};
