#include "mission_lost.h"

#include "game/callbacks.h"
#include "game/mission_lost.h"
#include "game/sounds.h"

void startMissionLost(MissionLost &) {}

void updateMissionLost(MissionLost &) {}

void missionLostMouseClicked(Game &game) {
  play(Sound::ok);
  loadCampaignMap(game);
}
