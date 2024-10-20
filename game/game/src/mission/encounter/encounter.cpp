#include "encounter.h"

#include "game/mission/encounter/encounter.h"
#include "game/mission/forces.h"
#include "game/mission/map.h"

static constexpr u16 peopleCountForHealth(u16 health) {
  return (health + 1) / 2;
}

static void initPeople(EncounterPerson people[maxEncounterPeoplePerSide],
                       u16 peopleCount, u16 baseXWords, s16 xIncrementWords) {
  for (u16 personIndex = 0; personIndex < maxEncounterPeoplePerSide;
       ++personIndex) {
    EncounterPerson *const person = &people[personIndex];
    person->alive = personIndex < peopleCount;
    person->xWords = baseXWords + (personIndex * xIncrementWords);
    person->y = 128 - (maxEncounterPeoplePerSide / 2) + personIndex;
  }
}

void startEncounter(Encounter &encounter, const MapUnit &attackingUnit,
                    const MapUnit &defendingUnit) {
  encounter.state = EncounterState::wait;
  encounter.frameCounter = 0;

  const bool attackerIsOnLeft = attackingUnit.force == Force::north;
  const u16 attackingPeopleCount = peopleCountForHealth(attackingUnit.health);
  const u16 defendingPeopleCount = peopleCountForHealth(defendingUnit.health);
  const u16 leftPeopleCount =
      attackerIsOnLeft ? attackingPeopleCount : defendingPeopleCount;
  const u16 rightPeopleCount =
      attackerIsOnLeft ? defendingPeopleCount : attackingPeopleCount;
  EncounterPerson(&leftPeople)[maxEncounterPeoplePerSide] =
      attackerIsOnLeft ? encounter.attackingPeople : encounter.defendingPeople;
  EncounterPerson(&rightPeople)[maxEncounterPeoplePerSide] =
      attackerIsOnLeft ? encounter.defendingPeople : encounter.attackingPeople;
  initPeople(leftPeople, leftPeopleCount, 10 - 2 - 2, -1);
  initPeople(rightPeople, rightPeopleCount, 10 + 2, 1);
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
