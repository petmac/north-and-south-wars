#include "draw_title.h"

#include "blit.h"
#include "chip.h"
#include "frame_fast.h"
#include "text.h"

#include "game/screen.h"
#include "game/title.h"

static constexpr u16 partHeight = 8;
static constexpr u16 y = screenHeight - partHeight - 12;

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

  switch ((title.frameIndex >> 7) & 15) {
  case 0:
  case 1:
    drawCenterText(background, "(c) Peter Mackay 2024");
    break;
  case 2:
    break;
  case 3:
  case 4:
    drawCenterText(background, "A small game for AmiGameJam 2024");
    break;
  case 5:
    break;
  case 6:
  case 7:
    drawCenterText(background, "- Click mouse button to start -");
    break;
  case 8:
    break;
  case 9:
    drawCenterText(background, "Greetings to the AmigaDev,");
    break;
  case 10:
    drawCenterText(background, "AmigaGameDev, and Official");
    break;
  case 11:
    drawCenterText(background, "Commodore Amiga Discords");
    break;
  case 12:
    break;
  case 13:
    drawCenterText(background, "Shout out to CharlieFar, NAG_Graham");
    break;
  case 14:
    drawCenterText(background, "& djh0ffman on Twitch");
    break;
  case 15:
    break;
  }
}
