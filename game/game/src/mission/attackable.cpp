#include "attackable.h"

#include "game/mission/attackable.h"
#include "game/mission/map.h"

void findAttackableUnits(Attackable &attackable, u16 indexOfAttackingUnit,
                         const Map &map) {
  attackable.unitCount = 0;

  for (u16 unitIndex = 0; unitIndex < map.unitCount; ++unitIndex) {
    // A unit can't attack itself
    if (unitIndex == indexOfAttackingUnit) {
      continue;
    }

    // TODO Skip dead units
    // TODO Skip units from the same force as the attacking unit

    attackable.unitIndices[attackable.unitCount] = unitIndex;
    ++attackable.unitCount;

    // Should never happen, but guard against buffer overflow
    if (attackable.unitCount >= maxAttackableUnits) {
      break;
    }
  }
}
