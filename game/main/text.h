#pragma once

#include <hardware/blit.h>  // BC0F_SRCA, etc.
#include <proto/graphics.h> // WaitBlit

template <u16 srcWidth, u16 srcHeight, u16 dstWidth, u16 dstHeight, u16 depth>
void drawText(InterleavedBitmap<dstWidth, dstHeight, depth> &dst,
              const MaskedInterleavedBitmap<srcWidth, srcHeight, depth> &font,
              u16 xBytes, u16 y, const char *text) {
  char ch = *text;
  if (ch == '\0') {
    return;
  }

  constexpr u16 srcWidthWords = (srcWidth + 15) / 16;
  constexpr u16 dstWidthWords = (dstWidth + 15) / 16;
  constexpr u16 bltamod = (srcWidthWords - 1 + srcWidthWords) * 2;
  constexpr u16 bltbmod = bltamod;
  constexpr u16 bltcmod = (dstWidthWords - 1) * 2;
  constexpr u16 bltdmod = bltcmod;
  constexpr u16 bltsize = ((depth * 8) << HSIZEBITS) | 1;

  auto &firstDstPlaneAsc = dst.rows[y].planes[0];
  auto &firstDstPlaneDesc = dst.rows[y + 7].planes[depth - 1];

  WaitBlit();
  custom.bltamod = bltamod;
  custom.bltalwm = 0xffff;
  custom.bltbmod = bltbmod;
  custom.bltcmod = bltcmod;
  custom.bltdmod = bltdmod;

  for (;;) {
    if (ch != ' ') {
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
      const auto &firstSrcPlaneAsc = font.rows[srcY].planes[0];
      const auto &firstSrcPlaneDesc = font.rows[srcY + 7].planes[depth - 1];
      const auto &firstSrcPlane = desc ? firstSrcPlaneDesc : firstSrcPlaneAsc;
      auto &firstDstPlane = desc ? firstDstPlaneDesc : firstDstPlaneAsc;

      const u16 bltcon0 = shift | BC0F_SRCA | BC0F_SRCB | BC0F_SRCC |
                          BC0F_DEST | ABC | ABNC | NABC | NANBC;
      const u16 bltcon1 = shift | (desc ? BC1F_DESC : 0);

      u16 *const bltapt =
          const_cast<u16 *>(&firstSrcPlane.maskWords[srcXWords]);
      const u16 bltafwm = srcXBytesIsOdd ? 0x00ff : 0xff00;
      u16 *const bltbpt =
          const_cast<u16 *>(&firstSrcPlane.imageWords[srcXWords]);
      u16 *const bltcpt = &firstDstPlane.words[xWords];
      u16 *const bltdpt = bltcpt;

      WaitBlit();
      custom.bltcon0 = bltcon0;
      custom.bltcon1 = bltcon1;
      custom.bltapt = bltapt;
      custom.bltafwm = bltafwm;
      custom.bltbpt = bltbpt;
      custom.bltcpt = bltcpt;
      custom.bltdpt = bltdpt;
      custom.bltsize = bltsize;
    }

    ++text;
    ch = *text;
    if (ch == '\0') {
      break;
    }

    ++xBytes;
  }
}
