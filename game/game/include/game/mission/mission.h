#pragma once

#include "attackable.h"
#include "map.h"
#include "pathfinding.h"

enum class MissionState : u8 {
  intro,
  startOfTurn,
  resupply,
  selectUnit,
  confirmEndTurn,
  selectUnitDestination,
  movingUnit,
  selectAttackOrWait,
  selectWait,
  selectTarget,
  encounter,
};

struct Mission {
  Map map;
  MissionState state;
  u16 selectedUnitIndex;
  Pathfinding pathfinding;
  TileCoords unitSource;
  TileCoords unitDestination;
  Attackable attackable;
};
