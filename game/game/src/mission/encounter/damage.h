#pragma once

#include "util/types.h"

enum class Terrain : u8;
enum class UnitType : u8;

u16 computeDamage(UnitType attackerType, u16 attackerHealth,
                  UnitType defenderType, u16 defenderHealth,
                  Terrain defenderTerrain);
