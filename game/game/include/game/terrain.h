#pragma once

#include "util/types.h"

// https://advancewars.fandom.com/wiki/Terrain
enum class Terrain : u8 {
  // Terrains
  road,
  plain,
  woods,
  mountain,
  river,
  bridge,
  sea,

  // TODO Buildings

  // Misc
  invalid,
};

extern const Terrain tileTerrain[256];
