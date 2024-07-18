#pragma once

#include "amiga/bitmap.h"
#include "amiga/custom.h"

#include <hardware/blit.h>  // BC0F_SRCA, etc.
#include <proto/graphics.h> // WaitBlit

template <u16 dstWidth, u16 dstHeight, u16 depth>
void clear(InterleavedBitmap<dstWidth, dstHeight, depth> &dst) {
  constexpr u16 dstWidthWords = (dstWidth + 15) / 16;
  constexpr u16 bltsize = ((1 * depth) << HSIZEBITS) | dstWidthWords;

  WaitBlit();
  custom.bltcon0 = BC0F_DEST | A_TO_D;
  custom.bltcon1 = 0;
  custom.bltadat = 0;
  custom.bltafwm = 0xffff;
  custom.bltalwm = 0xffff;
  custom.bltdmod = 0;

  for (u16 y = 0; y < dstHeight; ++y) {
    u16 *const bltdpt = &dst.rows[y].planes[0].words[0];

    WaitBlit();
    custom.bltdpt = bltdpt;
    custom.bltsize = bltsize;
  }
}

template <u16 srcWidth, u16 srcHeight, u16 dstWidth, u16 dstHeight, u16 depth>
void blitFast(InterleavedBitmap<dstWidth, dstHeight, depth> &dst,
              const InterleavedBitmap<srcWidth, srcHeight, depth> &src,
              u16 xWords, u16 y) {
  constexpr u16 srcWidthWords = (srcWidth + 15) / 16;
  constexpr u16 dstWidthWords = (dstWidth + 15) / 16;

  constexpr u16 bltcon0 = BC0F_SRCA | BC0F_DEST | A_TO_D;
  constexpr u16 bltcon1 = 0;

  u16 *const bltapt = const_cast<u16 *>(&src.rows[0].planes[0].words[0]);
  constexpr u16 bltamod = 0;

  u16 *const bltdpt = &dst.rows[y].planes[0].words[xWords];
  constexpr u16 bltdmod = (dstWidthWords - srcWidthWords) * 2;

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

template <u16 srcWidth, u16 srcHeight, u16 dstWidth, u16 dstHeight, u16 depth>
void blitFast(InterleavedBitmap<dstWidth, dstHeight, depth> &dst,
              const MaskedInterleavedBitmap<srcWidth, srcHeight, depth> &src,
              u16 xWords, u16 y) {
  constexpr u16 srcWidthWords = (srcWidth + 15) / 16;
  constexpr u16 dstWidthWords = (dstWidth + 15) / 16;

  constexpr u16 bltcon0 =
      BC0F_SRCA | BC0F_SRCB | BC0F_SRCC | BC0F_DEST | ABC | ABNC | NABC | NANBC;
  constexpr u16 bltcon1 = 0;

  u16 *const bltapt = const_cast<u16 *>(&src.rows[0].planes[0].maskWords[0]);
  constexpr u16 bltamod = srcWidthWords * 2;

  u16 *const bltbpt = const_cast<u16 *>(&src.rows[0].planes[0].imageWords[0]);
  constexpr u16 bltbmod = bltamod;

  u16 *const bltcpt = &dst.rows[y].planes[0].words[xWords];
  constexpr u16 bltcmod = (dstWidthWords - srcWidthWords) * 2;

  u16 *const bltdpt = bltcpt;
  constexpr u16 bltdmod = bltcmod;

  constexpr u16 bltsize = ((srcHeight * depth) << HSIZEBITS) | srcWidthWords;

  WaitBlit();
  custom.bltcon0 = bltcon0;
  custom.bltcon1 = bltcon1;
  custom.bltapt = bltapt;
  custom.bltamod = bltamod;
  custom.bltafwm = 0xffff;
  custom.bltalwm = 0xffff;
  custom.bltbpt = bltbpt;
  custom.bltbmod = bltbmod;
  custom.bltcpt = bltcpt;
  custom.bltcmod = bltcmod;
  custom.bltdpt = bltdpt;
  custom.bltdmod = bltdmod;
  custom.bltsize = bltsize;
}
