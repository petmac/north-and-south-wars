#pragma once

#include "playfield.h"

#include "amiga/bitmap.h"

using Background =
    InterleavedBitmap<playfieldWidth, playfieldHeight, playfieldDepth>;
