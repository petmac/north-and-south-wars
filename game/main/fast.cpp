#include "fast.h"

#include "amiga/custom.h"
#include "game/screen.h"

#include "gcc8_c_support.h" // KPrintF

static void initFrameFast(FrameFast &frameFast) {
  frameFast.state = FrameState::cleared;
  frameFast.intro.linesDrawn = 0;
  frameFast.intro.charsDrawn = 0;
}

void initFast(Fast &fast) {
  KPrintF("Size of fast = %lu", sizeof(fast));

  const u16 mouseData = custom.joy0dat;

  initGame(fast.game);
  fast.backBufferIndex = 0;
  fast.mouseX = screenWidth / 2;
  fast.mouseY = screenHeight / 2;
  fast.mouseClicked = false;
  fast.lastMouseDataX = mouseData & 0xff;
  fast.lastMouseDataY = (mouseData >> 8) & 0xff;
  fast.lastMouseLeftButton = false;

  for (u16 frameIndex = 0; frameIndex < 2; ++frameIndex) {
    initFrameFast(fast.frames[frameIndex]);
  }
}
