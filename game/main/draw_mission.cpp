#include "draw_mission.h"

#include "blit.h"
#include "chip.h"
#include "frame_fast.h"
#include "text.h"

#include "game/mission.h"

void drawMission(Background &background, FrameFast &frameFast,
                 const Mission &mission) {
  if (frameFast.state == FrameState::drawnMission) {
    return;
  }

  clear(background);
  drawText(background, chip.smallFont, 0, 0, "Mission");

  frameFast.state = FrameState::drawnMission;
}
