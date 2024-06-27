#pragma once

#include "amiga/bitmap.h"
#include "amiga/custom.h"

#include <hardware/blit.h>  // WaitBlit
#include <proto/graphics.h> // WaitBlit

template <u16 srcWidth, u16 srcHeight, u16 dstWidth, u16 dstHeight, u16 depth>
void blitFast(InterleavedBitmap<dstWidth, dstHeight, depth> &dst,
              const MaskedInterleavedBitmap<srcWidth, srcHeight, depth> &src) {
  constexpr u16 srcWidthWords = (srcWidth + 15) / 16;
  constexpr u16 dstWidthWords = (dstWidth + 15) / 16;

  constexpr u16 bltcon0 = BC0F_SRCA | BC0F_DEST | A_TO_D;
  constexpr u16 bltcon1 = 0;
  void *const bltapt = const_cast<u16 *>(&src.rows[0].planes[0].imageWords[0]);
  constexpr u16 bltamod = srcWidthWords * 2;
  constexpr u16 bltdmod = (dstWidthWords - srcWidthWords) * 2;
  void *const bltdpt = &dst.rows[0].planes[0].words[0];
  constexpr u16 bltsize = ((srcHeight * depth) << HSIZEBITS) | srcWidthWords;

  WaitBlit();
  custom.bltcon0 = bltcon0;
  custom.bltcon1 = bltcon1;
  custom.bltapt = bltapt;
  custom.bltamod = bltamod;
  custom.bltafwm = 0xffff;
  custom.bltalwm = 0xffff;
  custom.bltdpt = bltdpt;
  custom.bltdmod = bltdmod;
  custom.bltsize = bltsize;
}
