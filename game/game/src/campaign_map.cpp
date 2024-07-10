#include "campaign_map.h"

#include "game/campaign_map.h"

void startCampaignMap(CampaignMap &campaignMap) {}

void updateCampaignMap(CampaignMap &campaignMap, Game &game) {
  loadMission(game);
}
