#pragma once

#include "frame_chip.h"

struct Chip {
  FrameChip frames[2];
};

extern volatile Chip chip;
