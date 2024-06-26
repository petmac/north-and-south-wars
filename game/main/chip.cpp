#include "chip.h"

Chip chip __attribute__((section(".MEMF_CHIP")));

static void initFrameChip(FrameChip &frameChip) {
  Copper &copper = frameChip.copper;
  Background &background = frameChip.background;

  // Prep copperlist
  copper.screenScan = screenScanDefault();
  copper.setPlanes = copSetPlanes(&background);
  for (u16 colorIndex = 0; colorIndex < 32; ++colorIndex) {
    copper.colors[colorIndex] = copperMove(color[1], 0xff0);
  }
  copper.end = copperEnd();
}

void initChip() {
  for (u16 frameIndex = 0; frameIndex < 2; ++frameIndex) {
    FrameChip &frameChip = chip.frames[frameIndex];
    initFrameChip(frameChip);
  }
}
