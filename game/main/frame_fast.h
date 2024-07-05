#pragma once

#include "util/types.h"

enum class FrameState : u8 {
  cleared,
  drawnLoading,
  drawnIntro,
  drawnError,
};

struct IntroFrameState {
  u16 linesDrawn;
  u16 charsDrawn;
};

struct FrameFast {
  FrameState state;
  IntroFrameState intro;
};
