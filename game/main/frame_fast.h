#pragma once

#include "util/types.h"

enum class FrameState : u8 {
  cleared,
  drawnLoading,
  drawnIntro,
  drawnError,
};

struct FrameFast {
  FrameState state;
};
