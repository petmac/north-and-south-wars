#pragma once

#include "attackable.h"
#include "encounter/encounter.h"
#include "map.h"
#include "player_pathfinding.h"

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
  PlayerPathfinding playerPathfinding;
  TileCoords unitSource;
  TileCoords unitDestination;
  Attackable attackable;
  u16 defendingUnitIndex;
  Encounter encounter;
};
