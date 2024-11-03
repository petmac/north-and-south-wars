#pragma once

#include "../../playfield.h"

#include "amiga/bitmap.h"
#include "game/screen.h"

using TitleBitmap =
    InterleavedBitmap<screenWidth, screenHeight, playfieldDepth>;
