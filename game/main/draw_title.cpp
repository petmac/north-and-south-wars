#include "draw_title.h"

#include "blit.h"
#include "chip.h"
#include "frame_fast.h"
#include "text.h"

#include "game/title.h"

void drawTitle(Background &background, FrameFast &frameFast,
               const Title &title) {
  if (frameFast.state == FrameState::drawnTitle) {
    return;
  }

  clear(background);
  drawText(background, chip.main.smallFont, 0, 0, "Title");

  frameFast.state = FrameState::drawnTitle;
}
