#include "blit.h"
#include "chip.h"
#include "text.h"

#include "amiga/libs.h"
#include "amiga/mouse.h"
#include "amiga/system.h"
#include "game/game.h"

#include <hardware/dmabits.h>
#include <hardware/intbits.h>
#include <proto/dos.h>

static u8 backBufferIndex = 0;
static Game game;

static __attribute__((interrupt)) void interruptHandler() {
  // Acknowledge interrupt
  custom.intreq = INTF_VERTB;
  custom.intreq = INTF_VERTB; // reset vbl req. twice for a4000 bug.
}

static void runFrame() {
  updateGame(game);

  // Get back buffer
  FrameChip &frameChip = chip.frames[backBufferIndex];
  Copper &copper = frameChip.copper;
  Background &background = frameChip.background;

  // Draw to back buffer
  for (u16 i = 0; i < 8; ++i) {
    drawText(background, chip.smallFont, 2 + i, 16 + i * 8, "Hello, World!");
  }

  // Present back buffer
  WaitVbl();
  custom.cop1lc = reinterpret_cast<u32>(&copper);
  custom.dmacon = DMAF_SETCLR | DMAF_RASTER | DMAF_COPPER;

  // Swap front and back buffers
  backBufferIndex = 1 - backBufferIndex;
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

  game = initGame();
  initChip();

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
