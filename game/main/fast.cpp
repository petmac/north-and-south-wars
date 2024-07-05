#include "fast.h"

static void initFrameFast(FrameFast &frameFast) {
  frameFast.state = FrameState::cleared;
  frameFast.intro.linesDrawn = 0;
  frameFast.intro.charsDrawn = 0;
}

void initFast(Fast &fast) {
  fast.game = initGame();
  fast.backBufferIndex = 0;

  for (u16 frameIndex = 0; frameIndex < 2; ++frameIndex) {
    initFrameFast(fast.frames[frameIndex]);
  }
}
