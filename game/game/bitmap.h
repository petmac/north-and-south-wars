#pragma once

#include "util/types.h"

template <u16 width, u16 height, u16 depth> struct InterleavedBitmap {
  static constexpr u16 widthInWords = (width + 15) / 16;
  static constexpr u16 widthInBytes = widthInWords * 2;

  u16 words[height][depth][widthInWords];
};
