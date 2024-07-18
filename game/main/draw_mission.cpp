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
  blitFast(background, chip.tileset.tiles[0], 4, 16);
  blitFast(background, chip.tileset.tiles[1], 5, 32);
  blitFast(background, chip.tileset.tiles[2], 6, 48);
  blitFast(background, chip.tileset.tiles[3], 7, 64);

  frameFast.state = FrameState::drawnMission;
}
