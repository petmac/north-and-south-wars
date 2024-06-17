#pragma once

#include "types.h"

template <u16 width, u16 height, u16 depth> struct InterleavedBitmap {
  static constexpr u16 widthInWords = (width + 15) / 16;

  u16 words[height][depth][widthInWords];
};
