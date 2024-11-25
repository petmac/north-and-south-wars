#include "mission.h"

#include "attackable.h"
#include "encounter/encounter.h"
#include "menu.h"
#include "player_pathfinding.h"

#include "game/callbacks.h"
#include "game/mission/forces.h"
#include "game/mission/mission.h"
#include "game/mission/movement_cost.h" // maxCost
#include "game/mission/unit_defs.h"
#include "game/sounds.h"

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
    const MapUnit &unit = mission.map.units[unitIndex];

    // Is the unit dead? (empty tile)
    if (unit.health == 0) {
      continue;
    }

    // Is this unit on a different tile?
    if ((unit.coords.column != mouseCoords.column) ||
        (unit.coords.row != mouseCoords.row)) {
      continue;
    }

    // Is the unit from the wrong force?
    if (unit.force != Force::north) {
      // Don't register the click
      return;
    }

    // Has the unit already moved?
    if (unit.moved) {
      // Don't register the click
      return;
    }

    // Change state to unit destination selection
    play(Sound::ok);
    mission.state = MissionState::playerSelectUnitDestination;
    mission.selectedUnitIndex = unitIndex;

    // Initialise pathfinding
    const UnitDef &unitDef = unitDefForType(unit.type);
    findPaths(mission.playerPathfinding, mission.map, mouseCoords, unitDef);
    mission.unitSource = mouseCoords;
    mission.unitDestination = mouseCoords;
    return;
  }

  // Show "End Turn" menu
  play(Sound::ok);
  mission.state = MissionState::playerConfirmEndTurn;
}

static void waitSelected(Mission &mission) {
  play(Sound::ok);
  mission.map.units[mission.selectedUnitIndex].moved = true;
  mission.state = MissionState::playerSelectUnit;
}

static void cancelMove(Mission &mission) {
  play(Sound::cancel);
  mission.state = MissionState::playerSelectUnitDestination;
  // Move unit back
  MapUnit &selectedUnit = mission.map.units[mission.selectedUnitIndex];
  selectedUnit.coords = mission.unitSource;
}

static void selectTargetUnderMouse(Mission &mission, u16 mouseX, u16 mouseY) {
  // Which tile is the mouse over?
  const TileCoords mouseCoords = mouseTileCoords(mouseX, mouseY);
  Map &map = mission.map;

  // Look for an attackable unit in the tile the mouse is over
  for (u16 attackableUnitIndex = 0;
       attackableUnitIndex < mission.attackable.unitCount;
       ++attackableUnitIndex) {
    const u16 unitIndex = mission.attackable.unitIndices[attackableUnitIndex];
    const MapUnit &unit = map.units[unitIndex];

    // Skip enemies which are not under the mouse cursor
    if (unit.coords != mouseCoords) {
      continue;
    }

    // Change state to close up of encounter
    play(Sound::ok);
    play(Sound::zoomIn);
    mission.state = MissionState::playerEncounter;
    mission.defendingUnitIndex = unitIndex;
    MapUnit &attackingUnit = map.units[mission.selectedUnitIndex];
    attackingUnit.moved = true;
    startEncounter(mission.encounter, attackingUnit);
    return;
  }
}

static bool anyUnitsAlive(const Map &map, Force force) {
  for (u16 unitIndex = 0; unitIndex < map.unitCount; ++unitIndex) {
    const MapUnit &unit = map.units[unitIndex];
    if (unit.health == 0) {
      continue;
    }

    if (unit.force != force) {
      continue;
    }

    return true;
  }

  return false;
}

// Returns false if no movable units can be found
static bool indexOfEnemyUnitToMove(u16 *const movableUnitIndex,
                                   const Map &map) {
  for (u16 unitIndex = 0; unitIndex < map.unitCount; ++unitIndex) {
    const MapUnit &unit = map.units[unitIndex];
    if (unit.health == 0) {
      continue;
    }

    if (unit.force != Force::south) {
      continue;
    }

    if (unit.moved) {
      continue;
    }

    *movableUnitIndex = unitIndex;
    return true;
  }

  return false;
}

static u16 manhattanDistance(const TileCoords &a, const TileCoords &b) {
  const u16 horizontalDistance =
      a.column < b.column ? b.column - a.column : a.column - b.column;
  const u16 verticalDistance = a.row < b.row ? b.row - a.row : a.row - b.row;

  return horizontalDistance + verticalDistance;
}

// Returns false if no player units can be found
static bool indexOfNearestPlayerUnit(u16 *const playerUnitIndex, const Map &map,
                                     const TileCoords &coords) {
  bool found = false;
  u16 nearestDistance = 0;

  for (u16 unitIndex = 0; unitIndex < map.unitCount; ++unitIndex) {
    const MapUnit &unit = map.units[unitIndex];
    if (unit.health == 0) {
      continue;
    }

    if (unit.force != Force::north) {
      continue;
    }

    const u16 distance = manhattanDistance(unit.coords, coords);
    if (!found || distance < nearestDistance) {
      found = true;
      nearestDistance = distance;
      *playerUnitIndex = unitIndex;
    }
  }

  return found;
}

static void startMovingEnemyOrReturnToPlayerTurn(Mission &mission) {
  //  Start new day if all enemies have moved
  u16 selectedUnitIndex = 0;
  if (!indexOfEnemyUnitToMove(&selectedUnitIndex, mission.map)) {
    KPrintF("Can't find enemy to move, starting next day");
    mission.state = MissionState::startOfDay;
    return;
  }

  const MapUnit &selectedUnit = mission.map.units[selectedUnitIndex];

  u16 nearestPlayerUnitIndex = 0;
  if (!indexOfNearestPlayerUnit(&nearestPlayerUnitIndex, mission.map,
                                selectedUnit.coords)) {
    KPrintF("BUG: Can't find player to target, starting next day");
    mission.state = MissionState::startOfDay;
    return;
  }

  const MapUnit &nearestPlayerUnit = mission.map.units[nearestPlayerUnitIndex];
  const TileCoords unitDestination = {
      .column = static_cast<u8>(nearestPlayerUnit.coords.column + 1),
      .row = nearestPlayerUnit.coords.row,
  };

  // TODO Do pathfinding
  mission.state = MissionState::movingEnemyUnit;
  mission.selectedUnitIndex = selectedUnitIndex;
  mission.unitSource = selectedUnit.coords;
  mission.unitDestination = unitDestination;
}

void startMission(Mission &mission) {
  mission.state = MissionState::intro;
  for (u16 unitIndex = 0; unitIndex < mission.map.unitCount; ++unitIndex) {
    MapUnit *const unit = &mission.map.units[unitIndex];
    unit->health = 10;
  }
  mission.selectedUnitIndex = 0;
}

void updateMission(Mission &mission, u16 mouseX, u16 mouseY, Game &game) {
  switch (mission.state) {
  case MissionState::intro:
    mission.state = MissionState::startOfDay;
    break;
  case MissionState::startOfDay:
    mission.state = MissionState::resupply;
    break;
  case MissionState::resupply:
    mission.state = MissionState::playerSelectUnit;
    for (u16 unitIndex = 0; unitIndex < mission.map.unitCount; ++unitIndex) {
      mission.map.units[unitIndex].moved = false;
    }
    break;
  case MissionState::playerSelectUnit:
    break;
  case MissionState::playerConfirmEndTurn:
    break;
  case MissionState::playerSelectUnitDestination: {
    const TileCoords mouseCoords = mouseTileCoords(mouseX, mouseY);
    if (mission.playerPathfinding
            .costSoFar[mouseCoords.row][mouseCoords.column] >= maxCost) {
      break;
    }
    mission.unitDestination = mouseCoords;
  } break;
  case MissionState::movingPlayerUnit:
    // TODO Animate and move smoothly
    mission.map.units[mission.selectedUnitIndex].coords =
        mission.unitDestination;
    // Can the player attack?
    findAttackableUnits(mission.attackable, mission.selectedUnitIndex,
                        mission.map);
    if (mission.attackable.unitCount > 0) {
      mission.state = MissionState::playerSelectAttackOrWait;
    } else {
      mission.state = MissionState::playerSelectWait;
    }
    break;
  case MissionState::playerSelectAttackOrWait:
    break;
  case MissionState::playerSelectWait:
    break;
  case MissionState::playerSelectTarget:
    break;
  case MissionState::playerEncounter: {
    MapUnit &attackingUnit = mission.map.units[mission.selectedUnitIndex];
    MapUnit &defendingUnit = mission.map.units[mission.defendingUnitIndex];
    updateEncounter(mission.encounter, attackingUnit, defendingUnit, mission,
                    game);
  } break;
  case MissionState::movingEnemyUnit: {
    // TODO Animate and move smoothly
    mission.map.units[mission.selectedUnitIndex].coords =
        mission.unitDestination;
    // Can the enemy attack?
    findAttackableUnits(mission.attackable, mission.selectedUnitIndex,
                        mission.map);
    if (mission.attackable.unitCount == 0) {
      break;
    }

    // Change state to close up of encounter
    // TODO Don't always attack
    play(Sound::zoomIn);
    mission.state = MissionState::enemyEncounter;
    mission.defendingUnitIndex = mission.attackable.unitIndices[0];
    MapUnit &attackingUnit = mission.map.units[mission.selectedUnitIndex];
    attackingUnit.moved = true;
    startEncounter(mission.encounter, attackingUnit);
  } break;
  case MissionState::enemyEncounter:
    // TODO DRY, looks same as player encounter logic
    MapUnit &attackingUnit = mission.map.units[mission.selectedUnitIndex];
    MapUnit &defendingUnit = mission.map.units[mission.defendingUnitIndex];
    updateEncounter(mission.encounter, attackingUnit, defendingUnit, mission,
                    game);
    break;
  }
}

void missionMouseClicked(Mission &mission, u16 mouseX, u16 mouseY) {
  switch (mission.state) {
  case MissionState::intro:
  case MissionState::startOfDay:
  case MissionState::resupply:
    break;
  case MissionState::playerSelectUnit:
    selectUnitUnderMouse(mission, mouseX, mouseY);
    break;
  case MissionState::playerConfirmEndTurn:
    switch (menuButtonAtCoords(mouseX, mouseY, 1)) {
    case 0:
      // Start opponent's turn
      play(Sound::ok);
      startMovingEnemyOrReturnToPlayerTurn(mission);
      break;
    default:
      play(Sound::cancel);
      mission.state = MissionState::playerSelectUnit;
      break;
    }
    break;
  case MissionState::playerSelectUnitDestination: {
    // Clicked somewhere other than the destination?
    const TileCoords mouseCoords = mouseTileCoords(mouseX, mouseY);
    if (mouseCoords != mission.unitDestination) {
      play(Sound::cancel);
      mission.state = MissionState::playerSelectUnit;
      break;
    }
    play(Sound::ok);
    play(Sound::footsteps);
    mission.state = MissionState::movingPlayerUnit;
  } break;
  case MissionState::movingPlayerUnit:
    break;
  case MissionState::playerSelectAttackOrWait:
    switch (menuButtonAtCoords(mouseX, mouseY, 2)) {
    case 0:
      play(Sound::ok);
      mission.state = MissionState::playerSelectTarget;
      break;
    case 1:
      waitSelected(mission);
      break;
    default:
      cancelMove(mission);
      break;
    }
    break;
  case MissionState::playerSelectWait:
    switch (menuButtonAtCoords(mouseX, mouseY, 1)) {
    case 0:
      waitSelected(mission);
      break;
    default:
      cancelMove(mission);
      break;
    }
    break;
  case MissionState::playerSelectTarget:
    selectTargetUnderMouse(mission, mouseX, mouseY);
    break;
  case MissionState::playerEncounter:
  case MissionState::movingEnemyUnit:
  case MissionState::enemyEncounter:
    break;
  }
}

void missionMouseRightClicked(Mission &mission) {
  switch (mission.state) {
  case MissionState::intro:
  case MissionState::startOfDay:
  case MissionState::resupply:
  case MissionState::playerSelectUnit:
    break;
  case MissionState::playerConfirmEndTurn:
    play(Sound::cancel);
    mission.state = MissionState::playerSelectUnit;
    break;
  case MissionState::playerSelectUnitDestination:
    play(Sound::cancel);
    mission.state = MissionState::playerSelectUnit;
    break;
  case MissionState::movingPlayerUnit:
    break;
  case MissionState::playerSelectAttackOrWait:
  case MissionState::playerSelectWait:
    cancelMove(mission);
    break;
  case MissionState::playerSelectTarget:
    play(Sound::cancel);
    mission.state = MissionState::playerSelectAttackOrWait;
    break;
  case MissionState::playerEncounter:
  case MissionState::movingEnemyUnit:
  case MissionState::enemyEncounter:
    break;
  }
}

// Encounter callbacks
void encounterFinished(Mission &mission, Game &game) {
  play(Sound::zoomOut);

  const bool northAlive = anyUnitsAlive(mission.map, Force::north);
  const bool southAlive = anyUnitsAlive(mission.map, Force::south);

  if (northAlive) {
    if (southAlive) {
      switch (mission.state) {
      case MissionState::playerEncounter:
        // Still player turn
        KPrintF("Both forces still have units, still player's turn");
        mission.state = MissionState::playerSelectUnit;
        break;
      case MissionState::enemyEncounter:
        // Still enemy turn
        KPrintF("Both forces still have units, still enemy's turn");
        startMovingEnemyOrReturnToPlayerTurn(mission);
        break;
      default:
        // Neither force started the encounter, shouldn't happen
        KPrintF("BUG: Neither force started the encounter, shouldn't happen");
        break;
      }
    } else {
      // Player has won
      missionWon(game);
      return;
    }
  } else {
    if (southAlive) {
      // Player has lost
      missionLost(game);
      return;
    } else {
      // Both sides died, shouldn't happen
      KPrintF("BUG: Both sides died, shouldn't happen");
    }
  }
}
