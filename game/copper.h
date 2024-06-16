#pragma once

#include "custom.h"

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

struct ScreenScan {
  CopperMove ddfstrt;
  CopperMove ddfstop;
  CopperMove diwstrt;
  CopperMove diwstop;
};

struct SetPlanes {
  CopperMove bplcon0;
  CopperMove bplcon1;
  CopperMove bplcon2;
  CopperMove bpl1mod;
  CopperMove bpl2mod;
  CopperSetPointer bplpt[5];
};

struct Copper1 {
  ScreenScan screenScan;
  SetPlanes setPlanes;
  CopperMove colors[32];
  CopperMove copjmp2;
};

extern Copper1 copper1;
extern u16 copper2[];

constexpr CopperMove copperMoveImpl(u16 offset, u16 value) {
  return (CopperMove){
      .offset = offset,
      .value = value,
  };
}

#define copperMove(field, value) copperMoveImpl(customOffset(field), value)

constexpr CopperSetPointer copperSetPointerImpl(u16 offset, const void *p) {
  const u32 address = reinterpret_cast<u32>(p);

  return (CopperSetPointer){
      .hi = copperMoveImpl(offset, static_cast<u16>(address >> 16)),
      .lo = copperMoveImpl(offset + 2, address),
  };
}

#define copperSetPointer(field, p) copperSetPointerImpl(customOffset(field), p)

ScreenScan screenScanDefault();
SetPlanes copSetPlanes(const u8 **planes);

constexpr CopperMove copSetColor(u16 index, u16 color) {
  return copperMove(color[index], color);
}
