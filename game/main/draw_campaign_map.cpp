#include "draw_campaign_map.h"

#include "blit.h"
#include "chip.h"
#include "frame_fast.h"
#include "text.h"

#include "game/campaign_map.h"

void drawCampaignMap(Background &background, FrameFast &frameFast,
                     const CampaignMap &) {
  if (frameFast.state == FrameState::drawnCampaignMap) {
    return;
  }

  blitFast(background, chip.campaign.bitmap, 0, 0);
  drawText(background, chip.main.smallFont, 0, 0, "Campaign Map");

  frameFast.state = FrameState::drawnCampaignMap;
}
