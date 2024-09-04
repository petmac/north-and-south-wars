#include "mission.h"

#include "attackable.h"
#include "menu.h"
#include "pathfinding.h"

#include "game/mission/forces.h"
#include "game/mission/mission.h"
#include "game/mission/movement_cost.h" // maxCost
#include "game/mission/unit_defs.h"

static TileCoords mouseTileCoords(u16 mouseX, u16 mouseY) {
  const u16 tileColumn = mouseX / tileWidth;
  const u16 tileRow = mouseY / tileHeight;

  return (TileCoords){
      .column = static_cast<u8>(tileColumn),
      .row = static_cast<u8>(tileRow),
  };
}

static void selectUnitUnderMouse(Mission &mission, u16 mouseX, u16 mouseY) {
  // Which tile is the mouse over?
  const TileCoords mouseCoords = mouseTileCoords(mouseX, mouseY);

  // Look for a unit in the tile the mouse is over
  for (u16 unitIndex = 0; unitIndex < mission.map.unitCount; ++unitIndex) {
    const MapUnit &mapUnit = mission.map.units[unitIndex];

    // TODO Is the unit dead?

    // Is the unit from the wrong force?
    if (mapUnit.force != Force::north) {
      continue;
    }

    // Is this unit on a different tile?
    if ((mapUnit.coords.column != mouseCoords.column) ||
        (mapUnit.coords.row != mouseCoords.row)) {
      continue;
    }

    // Change state to unit destination selection
    mission.state = MissionState::selectUnitDestination;
    mission.selectedUnitIndex = unitIndex;

    // Initialise pathfinding
    const UnitDef &unitDef = unitDefForType(mapUnit.type);
    findPaths(mission.pathfinding, mission.map, mouseCoords, unitDef);
    mission.unitSource = mouseCoords;
    mission.unitDestination = mouseCoords;
    return;
  }

  // Show "End Turn" menu
  mission.state = MissionState::selectEndTurn;
}

static void cancelMove(Mission &mission) {
  mission.state = MissionState::selectUnitDestination;
  // Move unit back
  MapUnit &selectedUnit = mission.map.units[mission.selectedUnitIndex];
  selectedUnit.coords = mission.unitSource;
}

void startMission(Mission &mission) {
  mission.state = MissionState::intro;
  mission.selectedUnitIndex = 0;
}

void updateMission(Mission &mission, u16 mouseX, u16 mouseY) {
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
  case MissionState::selectEndTurn:
    break;
  case MissionState::selectUnitDestination: {
    const TileCoords mouseCoords = mouseTileCoords(mouseX, mouseY);
    if (mission.pathfinding.costSoFar[mouseCoords.row][mouseCoords.column] >=
        maxCost) {
      break;
    }
    mission.unitDestination = mouseCoords;
  } break;
  case MissionState::movingUnit:
    // TODO Animate and move smoothly
    mission.map.units[mission.selectedUnitIndex].coords =
        mission.unitDestination;
    // Can the player attack?
    findAttackableUnits(mission.attackable, mission.selectedUnitIndex,
                        mission.map);
    if (mission.attackable.unitCount > 0) {
      mission.state = MissionState::selectAttackOrWait;
    } else {
      mission.state = MissionState::selectWait;
    }
    break;
  case MissionState::selectAttackOrWait:
    break;
  case MissionState::selectWait:
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
  case MissionState::selectEndTurn:
    switch (menuButtonAtCoords(mouseX, mouseY, 1)) {
    case 0:
      // TODO Start opponent's turn
      mission.state = MissionState::resupply;
      break;
    default:
      mission.state = MissionState::selectUnit;
      break;
    }
    break;
  case MissionState::selectUnitDestination: {
    // Clicked somewhere other than the destination?
    const TileCoords mouseCoords = mouseTileCoords(mouseX, mouseY);
    if (mouseCoords != mission.unitDestination) {
      mission.state = MissionState::selectUnit;
      break;
    }
    mission.state = MissionState::movingUnit;
  } break;
  case MissionState::movingUnit:
    break;
  case MissionState::selectAttackOrWait:
    switch (menuButtonAtCoords(mouseX, mouseY, 2)) {
    case 0:
      mission.state = MissionState::selectTarget;
      break;
    case 1:
      // TODO Start opponent's turn
      mission.state = MissionState::resupply;
      break;
    default:
      cancelMove(mission);
      break;
    }
    break;
  case MissionState::selectWait:
    switch (menuButtonAtCoords(mouseX, mouseY, 1)) {
    case 0:
      // TODO Start opponent's turn
      mission.state = MissionState::resupply;
      break;
    default:
      cancelMove(mission);
      break;
    }
    break;
  case MissionState::selectTarget:
    break;
  }
}

void missionMouseRightClicked(Mission &mission) {
  switch (mission.state) {
  case MissionState::intro:
  case MissionState::startOfTurn:
  case MissionState::resupply:
  case MissionState::selectUnit:
    break;
  case MissionState::selectEndTurn:
    mission.state = MissionState::selectUnit;
    break;
  case MissionState::selectUnitDestination:
    mission.state = MissionState::selectUnit;
    break;
  case MissionState::movingUnit:
    break;
  case MissionState::selectAttackOrWait:
  case MissionState::selectWait:
    cancelMove(mission);
    break;
  case MissionState::selectTarget:
    mission.state = MissionState::selectAttackOrWait;
    break;
  }
}
