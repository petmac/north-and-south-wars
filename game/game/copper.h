#pragma once

#include "amiga/bitmap.h"
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
  CopperSetPointer bplpt[8];
};

struct Copper {
  ScreenScan screenScan;
  SetPlanes setPlanes;
  CopperMove colors[32];
  CopperWait end;
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

constexpr ScreenScan screenScanDefault() {
  constexpr u16 x = 129;
  constexpr u16 width = 320;
  constexpr u16 height = 256;
  constexpr u16 y = 44;
  constexpr u16 RES = 8; // 8=lowres,4=hires
  constexpr u16 xstop = x + width;
  constexpr u16 ystop = y + height;
  constexpr u16 fw = (x >> 1) - RES;

  return ScreenScan{
      .ddfstrt = copperMove(ddfstrt, fw),
      .ddfstop = copperMove(ddfstop, fw + (((width >> 4) - 1) << 3)),
      .diwstrt = copperMove(diwstrt, x + (y << 8)),
      .diwstop = copperMove(diwstop, (xstop - 256) + ((ystop - 256) << 8)),
  };
}

template <u16 width, u16 height, u16 depth>
constexpr SetPlanes
copSetPlanes(const InterleavedBitmap<width, height, depth> *bitmap) {
  constexpr u16 modulo = bitmap->widthInBytes * (depth - 1);

  SetPlanes ret = {
      .bplcon0 = copperMove(bplcon0, BPLCON0::COLOR | BPLCON0::BPU(depth)),
      .bplcon1 = copperMove(bplcon1, 0),
      .bplcon2 = copperMove(bplcon2, BPLCON2::PF2PRI),
      .bpl1mod = copperMove(bpl1mod, modulo),
      .bpl2mod = copperMove(bpl2mod, modulo),
      .bplpt = {},
  };

  for (u16 planeIndex = 0; planeIndex < depth; ++planeIndex) {
    ret.bplpt[planeIndex] =
        copperSetPointer(bplpt[planeIndex], &bitmap->words[0][planeIndex][0]);
  }

  for (u16 planeIndex = depth; planeIndex < 8; ++planeIndex) {
    ret.bplpt[planeIndex] = copperSetPointer(bplpt[planeIndex], nullptr);
  }

  return ret;
}

constexpr CopperMove copSetColor(u16 index, u16 color) {
  return copperMove(color[index], color);
}
