#pragma once

#include "unit_types.h"

struct UnitDef {
  u8 movement;
};

extern const UnitDef unitDefs[static_cast<u16>(UnitType::count)];
