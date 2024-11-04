#pragma once

#include "../../playfield.h"

#include "amiga/bitmap.h"
#include "game/screen.h"

using CampaignBitmap =
    InterleavedBitmap<screenWidth, screenHeight, playfieldDepth>;
