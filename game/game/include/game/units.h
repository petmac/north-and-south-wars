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

// https://warswiki.org/wiki/Advance_Wars#Units
constexpr UnitDef unitDefs[static_cast<u16>(UnitType::count)] = {
    // Infantry
    // https://warswiki.org/wiki/Infantry_(unit)
    {
        .movement = 3,
    },
    // Mech
    // https://warswiki.org/wiki/Mechanised_Infantry
    {
        .movement = 2,
    },
    // Light tank
    // https://warswiki.org/wiki/Tank
    {
        .movement = 6,
    },
    // Medium tank
    // https://warswiki.org/wiki/Medium_Tank
    {
        .movement = 5,
    },
};
