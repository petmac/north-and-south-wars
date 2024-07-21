#pragma once

#include "util/types.h"

template <u16 h> struct SpriteImage {
  static constexpr u16 height = h;

  u16 unknown;  // KingCon adds it
  u16 position; // Calculate using spritePosition()
  u16 control;  // Calculate using spriteControl()
  u16 data[height][2];
  u16 zeroes[2];
};

namespace SPRxPOS {
constexpr u16 VSTART_SHIFT = 8;
}; // namespace SPRxPOS

constexpr u16 spritePosition(u16 vStart, u16 hStart) {
  return (vStart << SPRxPOS::VSTART_SHIFT) | ((hStart >> 1) & 0xff);
}

namespace SPRxCTL {
constexpr u16 VSTOP_LO_SHIFT = 8;
constexpr u16 ATTACHED = 1 << 7;
constexpr u16 VSTART_HI = 1 << 2;
constexpr u16 VSTOP_HI = 1 << 1;
constexpr u16 HSTART_LO = 1 << 0;
}; // namespace SPRxCTL

constexpr u16 spriteControl(u16 vStart, u16 vStop, u16 hStart) {
  return ((vStop & 0xff) << SPRxCTL::VSTOP_LO_SHIFT) |
         ((vStart & 0x100) ? SPRxCTL::VSTART_HI : 0) |
         ((vStop & 0x100) ? SPRxCTL::VSTOP_HI : 0) |
         ((hStart & 1) ? SPRxCTL::HSTART_LO : 0);
}
