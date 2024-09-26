#pragma once

#include "util/types.h"

enum class EncounterState : u8 {
  wait,
};

struct Encounter {
  EncounterState state;
};
