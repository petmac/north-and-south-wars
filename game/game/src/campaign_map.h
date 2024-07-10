#pragma once

struct CampaignMap;
struct Game;

void startCampaignMap(CampaignMap &campaignMap);
void updateCampaignMap(CampaignMap &campaignMap, Game &game);

// Callbacks
void loadMission(Game &game);
