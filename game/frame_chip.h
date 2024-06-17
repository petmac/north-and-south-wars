#pragma once

#include "bitmap.h"

struct FrameChip {
  InterleavedBitmap<320, 256, 4> background;
};
