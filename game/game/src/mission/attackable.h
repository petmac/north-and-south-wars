#pragma once

#include "util/types.h"

struct Attackable;
struct Map;

void findAttackableUnits(Attackable &attackable, u16 indexOfAttackingUnit,
                         const Map &map);
