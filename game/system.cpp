#include "system.h"

#include "custom.h"
#include "libs.h"

#include "gcc8_c_support.h"

#include <exec/execbase.h>
#include <graphics/gfxbase.h>
#include <proto/exec.h>
#include <proto/graphics.h>

// backup
static UWORD SystemInts;
static UWORD SystemDMA;
static UWORD SystemADKCON;
static volatile APTR VBR = 0;
static APTR SystemIrq;
static View *ActiView;

static APTR GetVBR(void) {
  APTR vbr = 0;
  UWORD getvbr[] = {0x4e7a, 0x0801, 0x4e73}; // MOVEC.L VBR,D0 RTE

  if (SysBase->AttnFlags & AFF_68010)
    vbr = (APTR)Supervisor((ULONG(*)())getvbr);

  return vbr;
}

// vblank begins at vpos 312 hpos 1 and ends at vpos 25 hpos 1
// vsync begins at line 2 hpos 132 and ends at vpos 5 hpos 18
static void WaitVbl() {
  debug_start_idle();
  while (1) {
    volatile ULONG vpos = *(volatile ULONG *)0xDFF004;
    vpos &= 0x1ff00;
    if (vpos != (311 << 8))
      break;
  }
  while (1) {
    volatile ULONG vpos = *(volatile ULONG *)0xDFF004;
    vpos &= 0x1ff00;
    if (vpos == (311 << 8))
      break;
  }
  debug_stop_idle();
}

static APTR GetInterruptHandler() {
  return *(volatile APTR *)(((UBYTE *)VBR) + 0x6c);
}

void SetInterruptHandler(void *interrupt) {
  *(volatile APTR *)(((UBYTE *)VBR) + 0x6c) = interrupt;
}

void TakeSystem() {
  Forbid();
  // Save current interrupts and DMA settings so we can restore them upon exit.
  SystemADKCON = custom.adkconr;
  SystemInts = custom.intenar;
  SystemDMA = custom.dmaconr;
  ActiView = GfxBase->ActiView; // store current view

  LoadView(0);
  WaitTOF();
  WaitTOF();

  WaitVbl();
  WaitVbl();

  OwnBlitter();
  WaitBlit();
  Disable();

  custom.intena = 0x7fff; // disable all interrupts
  custom.intreq = 0x7fff; // Clear any interrupts that were pending

  custom.dmacon = 0x7fff; // Clear all DMA channels

  // set all colors black
  for (int a = 0; a < 32; a++)
    custom.color[a] = 0;

  WaitVbl();
  WaitVbl();

  VBR = GetVBR();
  SystemIrq = GetInterruptHandler(); // store interrupt register

  WaitVbl();
}

void FreeSystem() {
  WaitVbl();
  WaitBlit();
  custom.intena = 0x7fff; // disable all interrupts
  custom.intreq = 0x7fff; // Clear any interrupts that were pending
  custom.dmacon = 0x7fff; // Clear all DMA channels

  // restore interrupts
  SetInterruptHandler(SystemIrq);

  /*Restore system copper list(s). */
  custom.cop1lc = (ULONG)GfxBase->copinit;
  custom.cop2lc = (ULONG)GfxBase->LOFlist;
  custom.copjmp1 = 0x7fff; // start coppper

  /*Restore all interrupts and DMA settings. */
  custom.intena = SystemInts | 0x8000;
  custom.dmacon = SystemDMA | 0x8000;
  custom.adkcon = SystemADKCON | 0x8000;

  WaitBlit();
  DisownBlitter();
  Enable();

  LoadView(ActiView);
  WaitTOF();
  WaitTOF();

  Permit();
}
