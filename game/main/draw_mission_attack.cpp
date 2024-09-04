#include "draw_mission_attack.h"

#include "blit.h" // clear()
#include "frame_fast.h"

void drawMissionAttack(Background &background, FrameFast &frameFast,
                       const Mission &mission) {
  // Draw the background if needed.
  switch (frameFast.state) {
  case FrameState::drawnMissionAttack:
    // Already drawn
    break;
  default:
    // Need to draw the background
    // TODO Draw the background
    clear(background);
    frameFast.state = FrameState::drawnMissionAttack;
    break;
  }
}
