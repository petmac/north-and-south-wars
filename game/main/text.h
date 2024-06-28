#pragma once

#include <hardware/blit.h>  // BC0F_SRCA, etc.
#include <proto/graphics.h> // WaitBlit

template <u16 srcWidth, u16 srcHeight, u16 dstWidth, u16 dstHeight, u16 depth>
static void
drawChar(InterleavedBitmap<dstWidth, dstHeight, depth> &dst,
         const MaskedInterleavedBitmap<srcWidth, srcHeight, depth> &font,
         u16 xBytes, u16 y, char ch) {
  constexpr u16 srcWidthWords = (srcWidth + 15) / 16;
  constexpr u16 dstWidthWords = (dstWidth + 15) / 16;

  const u16 xWords = xBytes >> 1;
  const u16 glyphIndex = ch - ' ';
  const u16 srcXBytes = glyphIndex & 15;
  const u16 srcY = (glyphIndex >> 4) << 3;
  const u16 srcXWords = srcXBytes >> 1;
  const bool srcXBytesIsOdd = srcXBytes & 1;
  const bool dstXBytesIsOdd = xBytes & 1;
  const bool desc = srcXBytesIsOdd && !dstXBytesIsOdd;
  const bool needsShift = srcXBytesIsOdd != dstXBytesIsOdd;
  const u16 shift = needsShift ? (8 << ASHIFTSHIFT) : 0;
  const u16 firstPlaneIndex = desc ? depth - 1 : 0;
  const auto *const firstSrcPlane =
      &font.rows[desc ? srcY + 7 : srcY].planes[firstPlaneIndex];

  const u16 bltcon0 = shift | BC0F_SRCA | BC0F_SRCB | BC0F_SRCC | BC0F_DEST |
                      ABC | ABNC | NABC | NANBC;
  const u16 bltcon1 = shift | (desc ? BC1F_DESC : 0);

  u16 *const bltapt = const_cast<u16 *>(&firstSrcPlane->maskWords[srcXWords]);
  constexpr u16 bltamod = (srcWidthWords - 1 + srcWidthWords) * 2;
  const u16 bltafwm = srcXBytesIsOdd ? 0x00ff : 0xff00;

  u16 *const bltbpt = const_cast<u16 *>(&firstSrcPlane->imageWords[srcXWords]);
  constexpr u16 bltbmod = bltamod;

  u16 *const bltcpt =
      &dst.rows[desc ? y + 7 : y].planes[firstPlaneIndex].words[xWords];
  constexpr u16 bltcmod = (dstWidthWords - 1) * 2;

  u16 *const bltdpt = bltcpt;
  constexpr u16 bltdmod = bltcmod;

  constexpr u16 bltsize = ((depth * 8) << HSIZEBITS) | 1;

  WaitBlit();
  custom.bltcon0 = bltcon0;
  custom.bltcon1 = bltcon1;
  custom.bltapt = bltapt;
  custom.bltamod = bltamod;
  custom.bltafwm = bltafwm;
  custom.bltalwm = 0xffff;
  custom.bltbpt = bltbpt;
  custom.bltbmod = bltbmod;
  custom.bltcpt = bltcpt;
  custom.bltcmod = bltcmod;
  custom.bltdpt = bltdpt;
  custom.bltdmod = bltdmod;
  custom.bltsize = bltsize;
}

template <u16 srcWidth, u16 srcHeight, u16 dstWidth, u16 dstHeight, u16 depth>
void drawText(InterleavedBitmap<dstWidth, dstHeight, depth> &dst,
              const MaskedInterleavedBitmap<srcWidth, srcHeight, depth> &font,
              u16 xBytes, u16 y, const char *text) {
  char ch = *text;
  if (ch == '\0') {
    return;
  }

  for (;;) {
    if (ch != ' ') {
      drawChar(dst, font, xBytes, y, ch);
    }

    ++text;
    ch = *text;
    if (ch == '\0') {
      break;
    }

    ++xBytes;
  }
}
