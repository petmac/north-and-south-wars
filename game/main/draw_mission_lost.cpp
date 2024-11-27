#include "draw_mission_lost.h"

#include "blit.h"
#include "chip.h"
#include "frame_fast.h"
#include "text.h"

static constexpr u16 centerY = (256 - 8) / 2;
static constexpr u16 y1 = centerY - 8;
static constexpr u16 y2 = centerY + 8;
static constexpr char text1[] = "Mission Lost";
static constexpr char text2[] = "- Click mouse button to try again -";

void drawMissionLost(Background &background, FrameFast &frameFast,
                     const MissionLost &) {
  constexpr u16 x1 = (40 - sizeof(text1)) / 2;
  constexpr u16 x2 = (40 - sizeof(text2)) / 2;

  if (frameFast.state == FrameState::drawnMissionLost) {
    return;
  }

  clear(background);
  drawText(background, chip.main.smallFont, x1, y1, text1);
  drawText(background, chip.main.smallFont, x2, y2, text2);
  frameFast.state = FrameState::drawnMissionLost;
}
