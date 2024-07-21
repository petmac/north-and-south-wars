#include "fast.h"

#include "amiga/custom.h"
#include "game/screen.h"

static void initFrameFast(FrameFast &frameFast) {
  frameFast.state = FrameState::cleared;
  frameFast.intro.linesDrawn = 0;
  frameFast.intro.charsDrawn = 0;
}

void initFast(Fast &fast) {
  const u16 mouseData = custom.joy0dat;

  fast.game = initGame();
  fast.backBufferIndex = 0;
  fast.lastMouseDataX = mouseData & 0xff;
  fast.lastMouseDataY = (mouseData >> 8) & 0xff;
  fast.mouseX = screenWidth / 2;
  fast.mouseY = screenHeight / 2;

  for (u16 frameIndex = 0; frameIndex < 2; ++frameIndex) {
    initFrameFast(fast.frames[frameIndex]);
  }
}
