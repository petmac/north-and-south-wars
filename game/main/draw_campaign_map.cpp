#include "draw_campaign_map.h"

#include "blit.h"
#include "chip.h"
#include "frame_fast.h"
#include "text.h"

#include "game/campaign_map.h"

void drawCampaignMap(Background &background, FrameFast &frameFast,
                     const CampaignMap &campaignMap) {
  if (frameFast.state == FrameState::drawnCampaignMap) {
    return;
  }

  clear(background);
  drawText(background, chip.main.smallFont, 0, 0, "Campaign Map");

  frameFast.state = FrameState::drawnCampaignMap;
}
