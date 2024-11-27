#include "draw_title.h"

#include "blit.h"
#include "chip.h"
#include "frame_fast.h"
#include "text.h"

#include "game/screen.h"
#include "game/title.h"

static constexpr u16 partHeight = 8;
static constexpr u16 y = screenHeight - partHeight - 8;

template <u16 len>
static void drawCenterText(Background &background, const char (&text)[len]) {
  constexpr u16 xBytes = (40 - len) / 2;
  drawText(background, chip.main.smallFont, xBytes, y, text);
}

void drawTitle(Background &background, FrameFast &frameFast,
               const Title &title) {
  if (frameFast.state == FrameState::drawnTitle) {
    blitPartFast(background, chip.title.bitmap, 0, y, 0, y, screenWidth / 16,
                 partHeight);
  } else {
    blitFast(background, chip.title.bitmap, 0, 0);
    frameFast.state = FrameState::drawnTitle;
  }

  switch ((title.frameIndex >> 7) & 3) {
  case 0:
    drawCenterText(background, "(c) Peter Mackay 2024");
    break;
  case 1:
    drawCenterText(background, "A game for AmiGameJam 2024");
    break;
  case 2:
    drawCenterText(background, "- Click mouse button to start -");
    break;
  case 3:
    break;
  }
}
