#include "copper.h"
#include "custom.h"
#include "libs.h"
#include "mouse.h"
#include "system.h"

#include "gcc8_c_support.h"

#include <hardware/dmabits.h>
#include <hardware/intbits.h>
#include <proto/dos.h>
#include <proto/graphics.h>

static void WaitLine(u16 line) {
  while (((custom.vpos32 >> 8) & 511) != line) {
  }
}

// DEMO - INCBIN
static volatile short frameCounter = 0;
INCBIN(colors, "../game/image.pal")
INCBIN_CHIP(image,
            "../game/image.bpl") // load image into chipmem so we can use it
                                 // without copying
INCBIN_CHIP(bob, "../game/bob.bpl")

static u16 *scroll = nullptr;

static const u8 sinus15[] = {
    8,  8,  9,  10, 10, 11, 12, 12, 13, 13, 14, 14, 14, 15, 15, 15,
    15, 15, 15, 15, 14, 14, 14, 13, 13, 12, 12, 11, 10, 10, 9,  8,
    8,  7,  6,  5,  5,  4,  3,  3,  2,  2,  1,  1,  1,  0,  0,  0,
    0,  0,  0,  0,  1,  1,  1,  2,  2,  3,  3,  4,  5,  5,  6,  7,
};

static const u8 sinus40[] = {
    20, 22, 24, 26, 28, 30, 31, 33, 34, 36, 37, 38, 39, 39, 40, 40,
    40, 40, 39, 39, 38, 37, 36, 35, 34, 32, 30, 29, 27, 25, 23, 21,
    19, 17, 15, 13, 11, 10, 8,  6,  5,  4,  3,  2,  1,  1,  0,  0,
    0,  0,  1,  1,  2,  3,  4,  6,  7,  9,  10, 12, 14, 16, 18, 20,
};

static const u8 sinus32[] = {
    16, 18, 20, 22, 24, 25, 27, 28, 30, 30, 31, 32, 32, 32, 32, 31, 30,
    30, 28, 27, 25, 24, 22, 20, 18, 16, 14, 12, 10, 8,  7,  5,  4,  2,
    2,  1,  0,  0,  0,  0,  1,  2,  2,  4,  5,  7,  8,  10, 12, 14, 16,
};

static __attribute__((interrupt)) void interruptHandler() {
  custom.intreq = (1 << INTB_VERTB);
  custom.intreq = (1 << INTB_VERTB); // reset vbl req. twice for a4000 bug.

  // modify scrolling in copper list
  if (scroll) {
    int sin = sinus15[frameCounter & 63];
    *scroll = sin | (sin << 4);
  }

  // DEMO - increment frameCounter
  frameCounter++;
}

static void Wait10() { WaitLine(0x10); }

int main() {
  if (!loadLibs()) {
    Exit(0);
  }

  TakeSystem();

  scroll = &copper1.setPlanes.bplcon1.value;
  constexpr u16 lineSize = 320 / 8;

  // set bitplane pointers
  const u8 *planes[5];
  for (int a = 0; a < 5; a++) {
    planes[a] = (u8 *)image + lineSize * a;
  }

  copper1.screenScan = screenScanDefault();
  copper1.setPlanes = copSetPlanes(planes);

  // set colors
  for (int a = 0; a < 32; a++) {
    copper1.colors[a] = copSetColor(a, ((u16 *)colors)[a]);
  }

  // jump to copper2
  copper1.copjmp2 = copperMove(copjmp2, 0x7fff);

  custom.cop1lc = (u32)&copper1;
  custom.cop2lc = (u32)&copper2;
  custom.dmacon = DMAF_BLITTER; // disable blitter dma for copjmp bug
  custom.copjmp1 = 0x7fff;      // start coppper
  custom.dmacon =
      DMAF_SETCLR | DMAF_MASTER | DMAF_RASTER | DMAF_COPPER | DMAF_BLITTER;

  // DEMO
  SetInterruptHandler(interruptHandler);
  custom.intena = INTF_SETCLR | INTF_INTEN | INTF_VERTB;
  custom.intreq = (1 << INTB_VERTB); // reset vbl req

  while (!MouseLeft()) {
    Wait10();
    int f = frameCounter & 255;

    // clear
    WaitBlit();
    custom.bltcon0 = A_TO_D | DEST;
    custom.bltcon1 = 0;
    custom.bltadat = 0;
    custom.bltdpt = (u8 *)image + 320 / 8 * 200 * 5;
    custom.bltdmod = 0;
    custom.bltafwm = custom.bltalwm = 0xffff;
    custom.bltsize = ((56 * 5) << HSIZEBITS) | (320 / 16);

    // blit
    for (short i = 0; i < 16; i++) {
      const short x =
          i * 16 + sinus32[(frameCounter + i) % sizeof(sinus32)] * 2;
      const short y = sinus40[((frameCounter + i) * 2) & 63] / 2;
      u8 *src = (u8 *)bob + 32 / 8 * 10 * 16 * (i % 6);

      WaitBlit();
      custom.bltcon0 = 0xca | SRCA | SRCB | SRCC | DEST |
                       ((x & 15) << ASHIFTSHIFT); // A = source, B = mask, C =
                                                  // background, D = destination
      custom.bltcon1 = ((x & 15) << BSHIFTSHIFT);
      custom.bltapt = src;
      custom.bltamod = 32 / 8;
      custom.bltbpt = src + 32 / 8 * 1;
      custom.bltbmod = 32 / 8;
      custom.bltcpt = custom.bltdpt =
          (u8 *)image + 320 / 8 * 5 * (200 + y) + x / 8;
      custom.bltcmod = custom.bltdmod = (320 - 32) / 8;
      custom.bltafwm = custom.bltalwm = 0xffff;
      custom.bltsize = ((16 * 5) << HSIZEBITS) | (32 / 16);
    }
  }

  // END
  FreeSystem();

  unloadLibs();
}
