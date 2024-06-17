#include "copper.h"

// set up a 320x256 lowres display
ScreenScan screenScanDefault() {
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

SetPlanes copSetPlanes(const u8 **planes) {
  constexpr u16 lineSize = 320 / 8;

  return (SetPlanes){
      .bplcon0 = copperMove(bplcon0, BPLCON0::COLOR | BPLCON0::BPU(5)),
      .bplcon1 = copperMove(bplcon1, 0),
      .bplcon2 = copperMove(bplcon2, BPLCON2::PF2PRI),
      .bpl1mod = copperMove(bpl1mod, 4 * lineSize),
      .bpl2mod = copperMove(bpl2mod, 4 * lineSize),
      .bplpt =
          {
              copperSetPointer(bplpt[0], planes[0]),
              copperSetPointer(bplpt[1], planes[1]),
              copperSetPointer(bplpt[2], planes[2]),
              copperSetPointer(bplpt[3], planes[3]),
              copperSetPointer(bplpt[4], planes[4]),
          },
  };
}
