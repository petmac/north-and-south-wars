#include "campaign_map.h"

#include "dev.h"

#include "game/callbacks.h"
#include "game/campaign_map.h"
#include "game/sounds.h"

void startCampaignMap(CampaignMap &campaignMap) { campaignMap.frameIndex = 0; }

void updateCampaignMap(CampaignMap &campaignMap, Game &game) {
  // Continue?
  if (dev) {
    campaignMouseClicked(game);
    return;
  }

  ++campaignMap.frameIndex;
}

void campaignMouseClicked(Game &game) {
  play(Sound::ok);
  loadMission(game);
}
