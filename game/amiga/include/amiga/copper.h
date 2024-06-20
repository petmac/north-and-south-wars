#pragma once

#include "amiga/custom.h"

struct CopperMove {
  u16 offset;
  u16 value;
};

struct CopperWait {
  u16 ir1;
  u16 ir2;
};

struct CopperSetPointer {
  CopperMove hi;
  CopperMove lo;
};

constexpr CopperMove copperMoveImpl(u16 offset, u16 value) {
  return (CopperMove){
      .offset = offset,
      .value = value,
  };
}

#define copperMove(field, value) copperMoveImpl(customOffset(field), value)

constexpr CopperWait copperWait(u16 ir1, u16 ir2) {
  return (CopperWait){
      .ir1 = ir1,
      .ir2 = ir2,
  };
}

constexpr CopperWait copperEnd() { return copperWait(0xffff, 0xfffe); }

constexpr CopperSetPointer copperSetPointerImpl(u16 offset, const void *p) {
  const u32 address = reinterpret_cast<u32>(p);

  return (CopperSetPointer){
      .hi = copperMoveImpl(offset, static_cast<u16>(address >> 16)),
      .lo = copperMoveImpl(offset + 2, address),
  };
}

#define copperSetPointer(field, p) copperSetPointerImpl(customOffset(field), p)
