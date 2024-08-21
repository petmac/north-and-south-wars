#pragma once

#include "util/types.h"

constexpr u16 unitWidth = 16;
constexpr u16 unitHeight = 16;

// https://warswiki.org/wiki/Advance_Wars#Units
enum class UnitType : u8 {
  infantry,
  mech,
  lightTank,
  mediumTank,
  count,
};

struct UnitDef {
  u16 movement;
};

extern const UnitDef unitDefs[static_cast<u16>(UnitType::count)];
