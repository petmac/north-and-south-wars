#pragma once

#include "map.h"
#include "pathfinding.h"

enum class MissionState : u8 {
  intro,
  startOfTurn,
  resupply,
  selectUnit,
  selectEndTurn,
  selectUnitDestination,
  movingUnit,
  selectAttackOrWait,
  selectWait,
  selectTarget,
};

struct Mission {
  Map map;
  MissionState state;
  u16 selectedUnitIndex;
  Pathfinding pathfinding;
  TileCoords unitSource;
  TileCoords unitDestination;
};
