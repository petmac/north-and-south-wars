#pragma once

#include "map.h"

enum class MissionState : u8 {
  intro,
  startOfTurn,
  resupply,
  selectUnit,
  selectUnitDestination,
  movingUnit,
  selectUnitAction,
  selectTarget,
};

struct Mission {
  Map map;
  MissionState state;
};
