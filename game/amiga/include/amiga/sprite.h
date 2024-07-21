#pragma once

#include "util/types.h"

template <u16 height> struct SpriteImage {
  u16 position;
  u16 control;
  u16 data[height][2];
  u16 zeroes[2];
};
