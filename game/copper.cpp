#include "copper.h"

struct Copper2 {
  struct Line {
    CopperMove color;
    CopperWait wait;
  };

  Line lines[16];
};

Copper1 copper1 __attribute__((section(".MEMF_CHIP")));

// put copperlist into chip mem so we can use it without copying
Copper2 copper2 __attribute__((section(".MEMF_CHIP"))) = {
    .lines = {
        {.color = copSetColor(0, 0x0000), .wait = copperWait(0x4101, 0xff00)},
        {.color = copSetColor(0, 0x0111), .wait = copperWait(0x4201, 0xff00)},
        {.color = copSetColor(0, 0x0222), .wait = copperWait(0x4301, 0xff00)},
        {.color = copSetColor(0, 0x0333), .wait = copperWait(0x4401, 0xff00)},
        {.color = copSetColor(0, 0x0444), .wait = copperWait(0x4501, 0xff00)},
        {.color = copSetColor(0, 0x0555), .wait = copperWait(0x4601, 0xff00)},
        {.color = copSetColor(0, 0x0666), .wait = copperWait(0x4701, 0xff00)},
        {.color = copSetColor(0, 0x0777), .wait = copperWait(0x4801, 0xff00)},
        {.color = copSetColor(0, 0x0888), .wait = copperWait(0x4901, 0xff00)},
        {.color = copSetColor(0, 0x0999), .wait = copperWait(0x4a01, 0xff00)},
        {.color = copSetColor(0, 0x0aaa), .wait = copperWait(0x4b01, 0xff00)},
        {.color = copSetColor(0, 0x0bbb), .wait = copperWait(0x4c01, 0xff00)},
        {.color = copSetColor(0, 0x0ccc), .wait = copperWait(0x4d01, 0xff00)},
        {.color = copSetColor(0, 0x0ddd), .wait = copperWait(0x4e01, 0xff00)},
        {.color = copSetColor(0, 0x0eee), .wait = copperWait(0x4f01, 0xff00)},
        {.color = copSetColor(0, 0x0fff), .wait = copperWait(0xffff, 0xfffe)},
    }};

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
