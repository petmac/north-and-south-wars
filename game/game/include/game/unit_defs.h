#pragma once

#include "util/types.h"

enum class UnitType : u8;

struct UnitDef {
  u8 movement;
};

const UnitDef &unitDefForType(UnitType unitType);
