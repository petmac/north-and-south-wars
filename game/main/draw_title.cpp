#include "draw_title.h"

#include "blit.h"
#include "chip.h"
#include "frame_fast.h"

#include "game/title.h"

void drawTitle(Background &background, FrameFast &frameFast,
               const Title &title) {
  if (frameFast.state == FrameState::drawnTitle) {
    return;
  }

  blitFast(background, chip.title.bitmap, 0, 0);

  frameFast.state = FrameState::drawnTitle;
}
