#pragma once

#include "util/types.h"

// https://advancewars.fandom.com/wiki/Terrain
enum class Terrain : u8 {
  // Terrains
  bridge,
  mountain,
  plain,
  road,
  sea,
  woods,

  // TODO Buildings

  // Misc
  invalid,
};
