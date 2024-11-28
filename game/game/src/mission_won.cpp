#include "mission_won.h"

#include "game/callbacks.h"
#include "game/mission_won.h"
#include "game/sounds.h"

void startMissionWon(MissionWon &) {}

void updateMissionWon(MissionWon &) {}

void missionWonMouseClicked(Game &game) {
  play(Sound::ok);
  missionWonFinished(game);
}
