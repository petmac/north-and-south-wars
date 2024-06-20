#include "chip.h"
#include "copper.h"
#include "game.h"
#include "libs.h"
#include "system.h"

#include "amiga/mouse.h"

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
  background.words[0][0][0] = 0xffff;
  background.words[1][0][1] = 0xffff;
  background.words[2][1][2] = 0xffff;
  background.words[3][1][3] = 0xffff;

  // Prep copperlist
  copper.screenScan = screenScanDefault();
  copper.setPlanes = copSetPlanes(&frameChip.background);
  copper.colors[0] = copperMove(color[0], 0x124);
  copper.colors[1] = copperMove(color[1], 0xff0);
  copper.end = copperEnd();

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
  SetInterruptHandler(interruptHandler);

  custom.intena = INTF_SETCLR | INTF_INTEN | INTF_VERTB;
  custom.intreq = INTF_VERTB; // reset vbl req

  while (!MouseLeft()) {
    runFrame();
  }

  // END
  FreeSystem();
  unloadLibs();
}
