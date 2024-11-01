#include "audio.h"
#include "chip.h"
#include "draw.h"
#include "fast.h"

#include "amiga/libs.h"
#include "amiga/mouse.h"
#include "amiga/system.h"
#include "gcc8_c_support.h"

#include <hardware/dmabits.h>
#include <hardware/intbits.h>
#include <proto/dos.h>
#include <proto/exec.h> // AddIntServer()

static Fast fast;

static void updateMousePointerPosition() {
  // Read the input and update the game mouse coords
  const u16 mouseData = custom.joy0dat;
  const bool mouseLeftButton = mouseLeft();
  const bool mouseRightButton = mouseRight();
  const bool mouseClicked = mouseLeftButton && !fast.lastMouseLeftButton;
  const bool mouseRightClicked = mouseRightButton && !fast.lastMouseRightButton;
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
  fast.lastMouseLeftButton = mouseLeftButton;
  fast.lastMouseRightButton = mouseRightButton;

  fast.mouseX = mouseX;
  fast.mouseY = mouseY;
  fast.mouseClicked |= mouseClicked;
  fast.mouseRightClicked |= mouseRightClicked;

  // Update the sprite position
  const u16 hStart = 128 + fast.mouseX;
  const u16 vStart = 44 + fast.mouseY;
  const u16 vStop = vStart + MousePointerSpriteImage::height;
  chip.main.mousePointer.position = spritePosition(vStart, hStart);
  chip.main.mousePointer.control = spriteControl(vStart, vStop, hStart);
}

static inline void acknowledgeInterrupt(u16 flag) {
  // Acknowledge interrupt twice for A4000 bug
  custom.intreq = flag;
  custom.intreq = flag;
}

static void verticalBlankInterruptHandler() { updateMousePointerPosition(); }

static void audioInterruptHandler() {
  const u16 flags =
      custom.intreqr & (INTF_AUD0 | INTF_AUD1 | INTF_AUD2 | INTF_AUD3);

  if (flags & INTF_AUD0) {
    audioChannelInterruptHandler(0);
  }
  if (flags & INTF_AUD1) {
    audioChannelInterruptHandler(1);
  }
  if (flags & INTF_AUD2) {
    audioChannelInterruptHandler(2);
  }
  if (flags & INTF_AUD3) {
    audioChannelInterruptHandler(3);
  }

  acknowledgeInterrupt(flags);
}

static void runFrame() {
  // Get per-frame data
  FrameChip &frameChip = chip.frames[fast.backBufferIndex];
  FrameFast &frameFast = fast.frames[fast.backBufferIndex];

  // Draw
  drawGame(frameChip, frameFast, fast.game);

  // Handle mouse clicks
  if (fast.mouseClicked) {
    fast.mouseClicked = false;
    mouseClicked(fast.game, fast.mouseX, fast.mouseY);
  }
  if (fast.mouseRightClicked) {
    fast.mouseRightClicked = false;
    mouseRightClicked(fast.game);
  }

  // Update game while drawing finishes
  updateGame(fast.game, fast.mouseX, fast.mouseY);

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

  // Start with raster, copper and sprites disabled
  custom.cop1lc = 0;
  custom.cop2lc = 0;
  custom.dmacon = DMAF_RASTER | DMAF_COPPER | DMAF_SPRITE;

  initChip();
  initFast(fast);

  if (!audioChannelAlloc()) {
    Exit(0);
  }

  // Set up vertical blank interrupt handler
  fast.verticalBlankInterrupt.is_Node.ln_Type = NT_INTERRUPT;
  fast.verticalBlankInterrupt.is_Node.ln_Pri = -60;
  fast.verticalBlankInterrupt.is_Node.ln_Name = const_cast<char *>("VERTB");
  fast.verticalBlankInterrupt.is_Code = &verticalBlankInterruptHandler;
  AddIntServer(INTB_VERTB, &fast.verticalBlankInterrupt);

  // Set up the audio interrupt handlers
  Interrupt *prevAudioInterrupts[4] = {};
  for (u16 audioChannelIndex = 0; audioChannelIndex < 4; ++audioChannelIndex) {
    Interrupt *const interrupt = &fast.audioInterrupts[audioChannelIndex];

    interrupt->is_Node.ln_Type = NT_INTERRUPT;
    interrupt->is_Node.ln_Pri = 0;
    interrupt->is_Node.ln_Name = const_cast<char *>("AUD");
    interrupt->is_Code = &audioInterruptHandler;

    prevAudioInterrupts[audioChannelIndex] =
        SetIntVector(INTB_AUD0 + audioChannelIndex, interrupt);
  }

  custom.intena = INTF_SETCLR | INTF_AUD0 | INTF_AUD1 | INTF_AUD2 | INTF_AUD3;

  const u32 freeChipRAM = AvailMem(MEMF_CHIP | MEMF_LARGEST);
  const u32 freeFastRAM = AvailMem(MEMF_FAST | MEMF_LARGEST);

  KPrintF("Free chip RAM = %ld", freeChipRAM);
  KPrintF("Free fast RAM = %ld", freeFastRAM);

  for (;;) {
    runFrame();
  }

  // Restore interrupt handlers
  for (u16 audioChannelIndex = 0; audioChannelIndex < 4; ++audioChannelIndex) {
    SetIntVector(INTB_AUD0 + audioChannelIndex,
                 prevAudioInterrupts[audioChannelIndex]);
  }

  RemIntServer(INTB_VERTB, &fast.verticalBlankInterrupt);

  audioChannelFree();

  // END
  FreeSystem();
  unloadLibs();
}
