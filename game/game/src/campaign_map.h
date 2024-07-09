#pragma once

struct CampaignMap;
struct Game;

void updateCampaignMap(CampaignMap &campaignMap, Game &game);

// Callbacks
void startMission(Game &game);
