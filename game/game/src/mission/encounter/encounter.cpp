#include "encounter.h"

#include "game/mission/encounter/encounter.h"

void startEncounter(Encounter &encounter) {
  encounter.state = EncounterState::wait;
}

void updateEncounter(Encounter &encounter) {
  switch (encounter.state) {
  case EncounterState::wait:
    break;
  }
}
