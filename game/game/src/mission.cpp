#include "mission.h"

#include "game/mission.h"

static void selectUnitUnderMouse(Mission &mission, u16 mouseX, u16 mouseY) {
  // Which tile is the mouse over?
  const u16 tileColumn = mouseX / tileWidth;
  const u16 tileRow = mouseY / tileHeight;

  // Look for a unit in the tile the mouse is over
  for (u16 unitIndex = 0; unitIndex < mission.map.unitCount; ++unitIndex) {
    const MapUnit &mapUnit = mission.map.units[unitIndex];

    // TODO Is the unit dead?

    // Is this unit on a different tile?
    if ((mapUnit.coords.column != tileColumn) ||
        (mapUnit.coords.row != tileRow)) {
      continue;
    }

    // Change state to unit destination selection
    mission.state = MissionState::selectUnitDestination;
    mission.selectedUnitIndex = unitIndex;
    break;
  }
}

void startMission(Mission &mission) {
  mission.state = MissionState::intro;
  mission.selectedUnitIndex = 0;
}

void updateMission(Mission &mission, Game &game) {
  switch (mission.state) {
  case MissionState::intro:
    mission.state = MissionState::startOfTurn;
    break;
  case MissionState::startOfTurn:
    mission.state = MissionState::resupply;
    break;
  case MissionState::resupply:
    mission.state = MissionState::selectUnit;
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

void missionMouseClicked(Mission &mission, u16 mouseX, u16 mouseY) {
  switch (mission.state) {
  case MissionState::intro:
  case MissionState::startOfTurn:
  case MissionState::resupply:
    break;
  case MissionState::selectUnit:
    selectUnitUnderMouse(mission, mouseX, mouseY);
    break;
  case MissionState::selectUnitDestination:
  case MissionState::movingUnit:
  case MissionState::selectUnitAction:
  case MissionState::selectTarget:
    break;
  }
}
