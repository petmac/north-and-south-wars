#pragma once

struct Game;
struct MissionLost;

void startMissionLost(MissionLost &missionLost);
void updateMissionLost(MissionLost &missionLost);
void missionLostMouseClicked(Game &game);

// Callbacks
void loadCampaignMap(Game &game);
