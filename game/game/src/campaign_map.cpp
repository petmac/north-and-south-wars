#include "campaign_map.h"

#include "dev.h"

#include "game/callbacks.h"
#include "game/campaign_map.h"
#include "game/sounds.h"

void startCampaignMap(CampaignMap &campaignMap) {}

void updateCampaignMap(CampaignMap &campaignMap, Game &game) {
  // Continue?
  if (dev) {
    campaignMouseClicked(game);
    return;
  }
}

void campaignMouseClicked(Game &game) {
  play(Sound::ok);
  loadMission(game);
}
