#include "draw_mission_attack.h"

#include "blit.h" // clear()
#include "chip.h"
#include "frame_fast.h"

#include "game/screen.h" // screenWidth

static void drawBackground(Background &dst, const AttackBackground &src,
                           u16 xWords) {
  for (u16 sliceIndex = 0, y = 0; sliceIndex < attackBackgroundSliceCount;
       ++sliceIndex, y += attackBackgroundSliceHeight) {
    blitFast(dst, src[sliceIndex], xWords, y);
  }
}

void drawMissionAttack(Background &background, FrameFast &frameFast,
                       const Mission &mission) {
  // Draw the background if needed.
  switch (frameFast.state) {
  case FrameState::drawnMissionAttack:
    // Already drawn
    break;
  default:
    // Need to draw the background
    drawBackground(background, chip.mission.attack.backgrounds.plain, 0);
    drawBackground(background, chip.mission.attack.backgrounds.mountain,
                   screenWidth >> 5);
    frameFast.state = FrameState::drawnMissionAttack;
    break;
  }
}
