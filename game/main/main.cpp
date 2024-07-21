#include "chip.h"
#include "draw.h"
#include "fast.h"

#include "amiga/libs.h"
#include "amiga/system.h"

#include <hardware/dmabits.h>
#include <hardware/intbits.h>
#include <proto/dos.h>

static Fast fast;

static void updateMousePointerPosition() {
  // Read the input and update the game mouse coords
  const u16 mouseData = custom.joy0dat;
  const u8 mouseDataX = mouseData & 0xff;
  const u8 mouseDataY = (mouseData >> 8) & 0xff;
  const s8 mouseDX = mouseDataX - fast.lastMouseDataX;
  const s8 mouseDY = mouseDataY - fast.lastMouseDataY;
  s16 mouseX = fast.mouseX + mouseDX;
  s16 mouseY = fast.mouseY + mouseDY;
  if (mouseX < 0) {
    mouseX = 0;
  } else if (mouseX >= screenWidth) {
    mouseX = screenWidth - 1;
  }
  if (mouseY < 0) {
    mouseY = 0;
  } else if (mouseY >= screenHeight) {
    mouseY = screenHeight - 1;
  }
  fast.lastMouseDataX = mouseDataX;
  fast.lastMouseDataY = mouseDataY;
  fast.mouseX = mouseX;
  fast.mouseY = mouseY;

  // Update the sprite position
  const u16 hStart = 128 + fast.mouseX;
  const u16 vStart = 44 + fast.mouseY;
  const u16 vStop = vStart + MousePointerSpriteImage::height;
  chip.mousePointer.position = spritePosition(vStart, hStart);
  chip.mousePointer.control = spriteControl(vStart, vStop, hStart);
}

static __attribute__((interrupt)) void interruptHandler() {
  // Acknowledge interrupt
  custom.intreq = INTF_VERTB;
  custom.intreq = INTF_VERTB; // reset vbl req. twice for a4000 bug.
}

static void runFrame() {
  updateMousePointerPosition();

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
  custom.dmacon = DMAF_SETCLR | DMAF_RASTER | DMAF_COPPER | DMAF_SPRITE;

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

  for (;;) {
    runFrame();
  }

  // END
  FreeSystem();
  unloadLibs();
}
