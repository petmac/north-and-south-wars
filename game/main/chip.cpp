#include "chip.h"

#include "gcc8_c_support.h" // KPrintF

Chip chip __attribute__((section(".MEMF_CHIP")));

static void initFrameChip(FrameChip &frameChip) {
  Copper &copper = frameChip.copper;
  Background &background = frameChip.background;

  // Prep copperlist
  for (u16 spriteIndex = 0; spriteIndex < 8; ++spriteIndex) {
    copper.sprites[spriteIndex] =
        copperSetPointer(sprpt[spriteIndex], &chip.zeroes);
  }
  copper.sprites[7] =
      copperSetPointer(sprpt[7], &chip.main.mousePointer.position);
  copper.screenScan = screenScanDefault();
  copper.setPlanes = copSetPlanes(&background);
  for (u16 colorIndex = 0; colorIndex < 32; ++colorIndex) {
    copper.colors[colorIndex] = copperMove(color[colorIndex], 0xff0);
  }
  copper.end = copperEnd();
}

void initChip() {
  KPrintF("Size of chip = %ld", sizeof(chip));

  for (u16 frameIndex = 0; frameIndex < 2; ++frameIndex) {
    FrameChip &frameChip = chip.frames[frameIndex];
    initFrameChip(frameChip);
  }
}
