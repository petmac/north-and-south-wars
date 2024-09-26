#pragma once

#include "util/types.h"

enum class EncounterState : u8 {
  wait,
  attackerAttacking,
  defenderTakingDamage,
  defenderAttacking,
  attackerTakingDamage,
  done,
};

struct Encounter {
  EncounterState state;
  u16 frameCounter;
};
