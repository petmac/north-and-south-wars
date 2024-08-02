#pragma once

#include "util/types.h"

constexpr u16 unitWidth = 16;
constexpr u16 unitHeight = 16;

enum class UnitType : u8 {
  infantry,
  mech,
  lightTank,
  mediumTank,
  count,
};
