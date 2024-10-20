#include "encounter.h"

#include "damage.h"

#include "game/mission/encounter/encounter.h"
#include "game/mission/forces.h"
#include "game/mission/map.h"
#include "game/mission/mission.h"
#include "game/mission/tile.h"

static void initPeople(EncounterPerson people[maxEncounterPeoplePerSide],
                       u16 baseXWords, s16 xIncrementWords) {
  for (u16 personIndex = 0; personIndex < maxEncounterPeoplePerSide;
       ++personIndex) {
    EncounterPerson *const person = &people[personIndex];
    person->xWords = baseXWords + (personIndex * xIncrementWords);
    person->y = 128 - (maxEncounterPeoplePerSide / 2) + personIndex;
  }
}

static void takeDamage(MapUnit &defendingUnit, const MapUnit &attackingUnit,
                       const Mission &mission) {
  const TileIndex defenderTile =
      mission.map.tiles[defendingUnit.coords.row][defendingUnit.coords.column];
  const Terrain defenderTerrain = tileTerrain(defenderTile);
  const u16 damage =
      computeDamage(attackingUnit.type, attackingUnit.health,
                    defendingUnit.type, defendingUnit.health, defenderTerrain);
  defendingUnit.health -= damage;
}

u16 peopleCountForHealth(u16 health) { return (health + 1) / 2; }

void startEncounter(Encounter &encounter, const MapUnit &attackingUnit) {
  encounter.state = EncounterState::wait;
  encounter.frameCounter = 0;

  const bool attackerIsOnLeft = attackingUnit.force == Force::north;
  EncounterPerson(&leftPeople)[maxEncounterPeoplePerSide] =
      attackerIsOnLeft ? encounter.attackingPeople : encounter.defendingPeople;
  EncounterPerson(&rightPeople)[maxEncounterPeoplePerSide] =
      attackerIsOnLeft ? encounter.defendingPeople : encounter.attackingPeople;
  initPeople(leftPeople, 10 - 2 - 2, -1);
  initPeople(rightPeople, 10 + 2, 1);
}

void updateEncounter(Encounter &encounter, MapUnit &attackingUnit,
                     MapUnit &defendingUnit, Mission &mission) {
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
    takeDamage(defendingUnit, attackingUnit, mission);
    break;
  case EncounterState::defenderTakingDamage:
    encounter.frameCounter += 1;
    if (encounter.frameCounter < 50) {
      break;
    }
    if (defendingUnit.health == 0) {
      encounter.state = EncounterState::done;
      encounter.frameCounter = 0;
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
    takeDamage(attackingUnit, defendingUnit, mission);
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
