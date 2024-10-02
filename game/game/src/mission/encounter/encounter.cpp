#include "encounter.h"

#include "game/mission/encounter/encounter.h"

void startEncounter(Encounter &encounter, const MapUnit &attackingUnit,
                    const MapUnit &defendingUnit) {
  encounter.state = EncounterState::wait;
  encounter.frameCounter = 0;

  // TODO Place units
}

void updateEncounter(Encounter &encounter, const MapUnit &attackingUnit,
                     const MapUnit &defendingUnit, Mission &mission) {
  switch (encounter.state) {
  case EncounterState::wait:
    encounter.frameCounter += 1;
    if (encounter.frameCounter < 50) {
      break;
    }
    encounter.state = EncounterState::attackerAttacking;
    encounter.frameCounter = 0;
    break;
  case EncounterState::attackerAttacking:
    encounter.frameCounter += 1;
    if (encounter.frameCounter < 30) {
      break;
    }
    encounter.state = EncounterState::defenderTakingDamage;
    encounter.frameCounter = 0;
    break;
  case EncounterState::defenderTakingDamage:
    encounter.frameCounter += 1;
    if (encounter.frameCounter < 50) {
      break;
    }
    encounter.state = EncounterState::defenderAttacking;
    encounter.frameCounter = 0;
    break;
  case EncounterState::defenderAttacking:
    encounter.frameCounter += 1;
    if (encounter.frameCounter < 30) {
      break;
    }
    encounter.state = EncounterState::attackerTakingDamage;
    encounter.frameCounter = 0;
    break;
  case EncounterState::attackerTakingDamage:
    encounter.frameCounter += 1;
    if (encounter.frameCounter < 50) {
      break;
    }
    encounter.state = EncounterState::done;
    encounter.frameCounter = 0;
    break;
  case EncounterState::done:
    encounter.frameCounter += 1;
    if (encounter.frameCounter < 50) {
      break;
    }
    encounterFinished(mission);
    break;
  }
}
