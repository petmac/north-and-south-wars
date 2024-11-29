#include "draw_campaign_map.h"

#include "blit.h"
#include "chip.h"
#include "frame_fast.h"
#include "text.h"

#include "game/campaign_map.h"
#include "game/missions.h"

struct FlagPos {
  u16 xWords;
  u16 y;
};

static constexpr FlagPos flags[missionCount] = {
    {
      xWords : 14,
      y : 95,
    },
    {
      xWords : 15,
      y : 120,
    },
    {
      xWords : 14,
      y : 143,
    },
    {
      xWords : 13,
      y : 160,
    },
};

void drawCampaignMap(Background &background, FrameFast &frameFast,
                     const CampaignMap &campaignMap) {
  switch (frameFast.state) {
  case FrameState::drawnCampaignMap:
    // Restore background under flags
    for (u16 flagIndex = 0; flagIndex < missionCount; ++flagIndex) {
      const FlagPos &flag = flags[flagIndex];
      blitPartFast(background, chip.campaign.bitmap, flag.xWords, flag.y,
                   flag.xWords, flag.y, (flagWidth + 15) / 16, flagHeight);
    }
    break;
  default:
    // Draw whole background
    blitFast(background, chip.campaign.bitmap, 0, 0);
    drawText(background, chip.main.smallFont, (40 - 12) / 2, 8, "Campaign Map");
    frameFast.state = FrameState::drawnCampaignMap;
    break;
  }

  for (u16 flagIndex = 0; flagIndex < missionCount; ++flagIndex) {
    const FlagPos &flag = flags[flagIndex];
    const u16 flagFrameIndex =
        ((campaignMap.frameIndex + (flagIndex << 2)) >> 3) % flagFrameCount;

    blitFast(background, chip.campaign.flags[0][flagFrameIndex], flag.xWords,
             flag.y);
  }
}
