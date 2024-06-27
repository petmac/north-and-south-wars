#include "chip.h"

Chip chip __attribute__((section(".MEMF_CHIP")));

static void initFrameChip(FrameChip &frameChip) {
  Copper &copper = frameChip.copper;
  Background &background = frameChip.background;

  // Fill background
  for (u16 rowIndex = 0; rowIndex < 256; ++rowIndex) {
    const u16 colorIndex = rowIndex & 31;
    for (u16 planeIndex = 0; planeIndex < 5; ++planeIndex) {
      const u16 word = (colorIndex & (1 << planeIndex)) ? 0xffff : 0x0000;
      for (u16 wordIndex = 0; wordIndex < (320 / 16); ++wordIndex) {
        background.rows[rowIndex].planes[planeIndex].words[wordIndex] = word;
      }
    }
  }

  // Prep copperlist
  copper.screenScan = screenScanDefault();
  copper.setPlanes = copSetPlanes(&background);
  for (u16 colorIndex = 0; colorIndex < 32; ++colorIndex) {
    copper.colors[colorIndex] = copperMove(color[colorIndex], 0xff0);
  }
  copper.end = copperEnd();
}

void initChip() {
  for (u16 frameIndex = 0; frameIndex < 2; ++frameIndex) {
    FrameChip &frameChip = chip.frames[frameIndex];
    initFrameChip(frameChip);
  }
}
