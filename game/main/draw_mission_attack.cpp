#include "draw_mission_attack.h"

#include "blit.h" // clear()
#include "chip.h"
#include "frame_fast.h"

#include "game/screen.h" // screenWidth

static void drawBackgroundSlices(Background &dst, const AttackBackground &src,
                                 u16 xWords, u16 firstSlice,
                                 u16 sliceAfterLast) {
  for (u16 sliceIndex = firstSlice,
           y = firstSlice * attackBackgroundSliceHeight;
       sliceIndex < sliceAfterLast;
       ++sliceIndex, y += attackBackgroundSliceHeight) {
    blitFast(dst, src[sliceIndex], xWords, y);
  }
}

static void drawBackground(Background &dst, const AttackBackground &src,
                           u16 xWords) {
  drawBackgroundSlices(dst, src, xWords, 0, attackBackgroundSliceCount);
}

void drawMissionAttack(Background &background, FrameFast &frameFast,
                       const Mission &mission) {
  // TODO Get these from the terrain types.
  const AttackBackground &leftBackground =
      chip.mission.attack.backgrounds.plain;
  const AttackBackground &rightBackground =
      chip.mission.attack.backgrounds.mountain;
  constexpr u16 leftXWords = 0;
  constexpr u16 rightXWords = screenWidthInWords / 2;

  // Draw the background if needed.
  switch (frameFast.state) {
  case FrameState::drawnMissionAttack: {
    // Already drawn similar, so just overwrite the animating parts
    constexpr u16 firstSlice = (attackBackgroundSliceCount / 2) - 1;
    constexpr u16 sliceAfterLast = firstSlice + 2;
    drawBackgroundSlices(background, leftBackground, leftXWords, firstSlice,
                         sliceAfterLast);
    drawBackgroundSlices(background, rightBackground, rightXWords, firstSlice,
                         sliceAfterLast);
  } break;
  default:
    // Draw the whole background
    drawBackground(background, leftBackground, leftXWords);
    drawBackground(background, rightBackground, rightXWords);
    frameFast.state = FrameState::drawnMissionAttack;
    break;
  }

  // Draw the units
  blitFast(background, chip.mission.attack.units.forces[0].units[0], 5, 128);
  blitFast(background, chip.mission.attack.units.forces[1].units[0], 15, 128);
}
