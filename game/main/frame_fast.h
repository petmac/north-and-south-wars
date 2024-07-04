#pragma once

#include "util/types.h"

enum class FrameState : u8 {
  cleared,
};

struct FrameFast {
  FrameState state;
};
