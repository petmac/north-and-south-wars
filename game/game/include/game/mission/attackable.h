#pragma once

#include "util/types.h"

constexpr u16 maxAttackableUnits = 4;

struct Attackable {
  u16 unitCount;
  u16 unitIndices[maxAttackableUnits];
};
