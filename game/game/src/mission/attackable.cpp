#include "attackable.h"

#include "game/mission/attackable.h"
#include "game/mission/map.h"
#include "game/mission/unit.h"

static u16 dist(u16 a, u16 b) {
  if (a < b) {
    return b - a;
  } else {
    return a - b;
  }
}

static u16 dist(const TileCoords &a, const TileCoords &b) {
  return dist(a.row, b.row) + dist(a.column, b.column);
}

void findAttackableUnits(Attackable &attackable, u16 indexOfAttackingUnit,
                         const Map &map, const UnitInstance units[]) {
  const MapUnit &attackingUnit = map.units[indexOfAttackingUnit];

  attackable.unitCount = 0;

  for (u16 unitIndex = 0; unitIndex < map.unitCount; ++unitIndex) {
    // A unit can't attack itself
    if (unitIndex == indexOfAttackingUnit) {
      continue;
    }

    // Skip dead units
    const UnitInstance &unit = units[unitIndex];
    if (unit.health == 0) {
      continue;
    }

    // Skip units from the same force as the attacking unit
    const MapUnit &mapUnit = map.units[unitIndex];
    if (mapUnit.force == attackingUnit.force) {
      continue;
    }

    // Skip units that are too far away to be attacked
    if (dist(attackingUnit.coords, mapUnit.coords) > 1) {
      continue;
    }

    // Add this unit to the attackable list
    attackable.unitIndices[attackable.unitCount] = unitIndex;
    ++attackable.unitCount;

    // Should never happen, but guard against buffer overflow
    if (attackable.unitCount >= maxAttackableUnits) {
      break;
    }
  }
}
