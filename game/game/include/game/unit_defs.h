#pragma once

#include "util/types.h"

enum class MovementType : u8;
enum class UnitType : u8;

struct UnitDef {
  u8 movementPoints;
  MovementType movementType;
};

const UnitDef &unitDefForType(UnitType unitType);
