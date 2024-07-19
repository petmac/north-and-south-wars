#include "mission.h"

#include "game/mission.h"

void startMission(Mission &mission) { mission.state = MissionState::intro; }

void updateMission(Mission &mission, Game &game) {
  switch (mission.state) {
  case MissionState::intro:
    break;
  case MissionState::startOfTurn:
    break;
  case MissionState::resupply:
    break;
  case MissionState::selectUnit:
    break;
  case MissionState::selectUnitDestination:
    break;
  case MissionState::movingUnit:
    break;
  case MissionState::selectUnitAction:
    break;
  case MissionState::selectTarget:
    break;
  }
}
