#include "chip.h"
#include "draw.h"
#include "fast.h"

#include "amiga/libs.h"
#include "amiga/mouse.h"
#include "amiga/system.h"

#include <hardware/dmabits.h>
#include <hardware/intbits.h>
#include <proto/dos.h>

static Fast fast;

static __attribute__((interrupt)) void interruptHandler() {
  // Acknowledge interrupt
  custom.intreq = INTF_VERTB;
  custom.intreq = INTF_VERTB; // reset vbl req. twice for a4000 bug.
}

static void runFrame() {
  // Get per-frame data
  FrameChip &frameChip = chip.frames[fast.backBufferIndex];
  FrameFast &frameFast = fast.frames[fast.backBufferIndex];

  // Draw
  drawGame(frameChip, frameFast, fast.game);

  // Update game while drawing finishes
  updateGame(fast.game);

  // Present back buffer
  WaitVbl();
  custom.cop1lc = reinterpret_cast<u32>(&frameChip.copper);
  custom.dmacon = DMAF_SETCLR | DMAF_RASTER | DMAF_COPPER;

  // Swap front and back buffers
  fast.backBufferIndex = 1 - fast.backBufferIndex;
}

int main() {
  if (!loadLibs()) {
    Exit(0);
  }

  TakeSystem();

  // Start with display disabled
  custom.cop1lc = 0;
  custom.cop2lc = 0;
  custom.dmacon = DMAF_SETCLR | DMAF_MASTER | DMAF_BLITTER;

  initChip();
  initFast(fast);

  SetInterruptHandler(interruptHandler);

  custom.intena = INTF_SETCLR | INTF_INTEN | INTF_VERTB | INTF_PORTS;
  custom.intreq = INTF_VERTB; // reset vbl req

  while (!MouseLeft()) {
    runFrame();
  }

  // END
  FreeSystem();
  unloadLibs();
}
