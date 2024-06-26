#pragma once

#include "copper.h"

using Background = InterleavedBitmap<320, 256, 5>;

struct FrameChip {
  Copper copper;
  Background background;
};
