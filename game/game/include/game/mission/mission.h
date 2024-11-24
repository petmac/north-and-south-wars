#pragma once

#include "attackable.h"
#include "encounter/encounter.h"
#include "map.h"
#include "pathfinding.h"

enum class MissionState : u8 {
  intro,
  startOfDay,
  resupply,
  playerSelectUnit,
  playerConfirmEndTurn,
  playerSelectUnitDestination,
  movingPlayerUnit,
  playerSelectAttackOrWait,
  playerSelectWait,
  playerSelectTarget,
  playerEncounter,
  movingEnemyUnit,
  enemyEncounter,
};

struct Mission {
  Map map;
  MissionState state;
  u16 selectedUnitIndex;
  Pathfinding pathfinding;
  TileCoords unitSource;
  TileCoords unitDestination;
  Attackable attackable;
  u16 defendingUnitIndex;
  Encounter encounter;
};
