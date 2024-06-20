#pragma once

#include "copper.h"

using Background = InterleavedBitmap<320, 256, 4>;

struct FrameChip {
  Copper copper;
  Background background;
};
