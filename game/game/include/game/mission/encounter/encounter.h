#pragma once

#include "util/types.h"

constexpr u16 maxEncounterPeoplePerSide = 5; // TODO Relate to max health?

enum class EncounterState : u8 {
  wait,
  attackerAttacking,
  defenderTakingDamage,
  defenderAttacking,
  attackerTakingDamage,
  done,
};

struct EncounterPerson {
  bool alive;
  u16 xWords;
  u16 y;
};

struct Encounter {
  EncounterState state;
  u16 frameCounter;
  EncounterPerson attackingPeople[maxEncounterPeoplePerSide];
  EncounterPerson defendingPeople[maxEncounterPeoplePerSide];
};
