#pragma once

#include "types.h"

#include <hardware/custom.h>

// http://www.winnicki.net/amiga/memmap/BPLCON0.html
namespace BPLCON0 {
constexpr u16 HIRES = 1 << 15;
constexpr u16 BPU2 = 1 << 14;
constexpr u16 BPU1 = 1 << 13;
constexpr u16 BPU0 = 1 << 12;
constexpr u16 HAM = 1 << 11;
constexpr u16 DPF = 1 << 10;
constexpr u16 COLOR = 1 << 9;
constexpr u16 GAUD = 1 << 8;
constexpr u16 UHRES = 1 << 7;
constexpr u16 SHRES = 1 << 6;
constexpr u16 BYPASS = 1 << 5;
constexpr u16 BPU3 = 1 << 4;
constexpr u16 LPEN = 1 << 3;
constexpr u16 LACE = 1 << 2;
constexpr u16 ERSY = 1 << 1;
constexpr u16 ECSENA = 1 << 0;

constexpr u16 BPU(u16 planeCount) {
  return ((planeCount & 1) ? BPU0 : 0) | ((planeCount & 2) ? BPU1 : 0) |
         ((planeCount & 4) ? BPU2 : 0) | ((planeCount & 8) ? BPU3 : 0);
}
} // namespace BPLCON0

// http://amiga-dev.wikidot.com/hardware:bplcon1
namespace BPLCON1 {
constexpr u16 PF2H7 = 1 << 15;
constexpr u16 PF2H6 = 1 << 14;
constexpr u16 PF2H1 = 1 << 13;
constexpr u16 PF2H0 = 1 << 12;
constexpr u16 PF1H7 = 1 << 11;
constexpr u16 PF1H6 = 1 << 10;
constexpr u16 PF1H1 = 1 << 9;
constexpr u16 PF1H0 = 1 << 8;
constexpr u16 PF2H5 = 1 << 7;
constexpr u16 PF2H4 = 1 << 6;
constexpr u16 PF2H3 = 1 << 5;
constexpr u16 PF2H2 = 1 << 4;
constexpr u16 PF1H5 = 1 << 3;
constexpr u16 PF1H4 = 1 << 2;
constexpr u16 PF1H3 = 1 << 1;
constexpr u16 PF1H2 = 1 << 0;

constexpr u16 PF1H(u16 scrollCode) {
  return ((scrollCode & 1) ? PF1H0 : 0) | ((scrollCode & 2) ? PF1H1 : 0) |
         ((scrollCode & 4) ? PF1H2 : 0) | ((scrollCode & 8) ? PF1H3 : 0) |
         ((scrollCode & 16) ? PF1H4 : 0) | ((scrollCode & 32) ? PF1H5 : 0) |
         ((scrollCode & 64) ? PF1H6 : 0) | ((scrollCode & 128) ? PF1H7 : 0);
}

constexpr u16 PF2H(u16 scrollCode) {
  return ((scrollCode & 1) ? PF2H0 : 0) | ((scrollCode & 2) ? PF2H1 : 0) |
         ((scrollCode & 4) ? PF2H2 : 0) | ((scrollCode & 8) ? PF2H3 : 0) |
         ((scrollCode & 16) ? PF2H4 : 0) | ((scrollCode & 32) ? PF2H5 : 0) |
         ((scrollCode & 64) ? PF2H6 : 0) | ((scrollCode & 128) ? PF2H7 : 0);
}
} // namespace BPLCON1

// http://amiga-dev.wikidot.com/hardware:bplcon2
namespace BPLCON2 {
constexpr u16 ZDBPSEL2 = 1 << 14;
constexpr u16 ZDBPSEL1 = 1 << 13;
constexpr u16 ZDBPSEL0 = 1 << 12;
constexpr u16 ZDBPEN = 1 << 11;
constexpr u16 ZDCTEN = 1 << 10;
constexpr u16 KILLEHB = 1 << 9;
constexpr u16 RDRAM = 1 << 8;
constexpr u16 SOGEN = 1 << 7;
constexpr u16 PF2PRI = 1 << 6;
constexpr u16 PF2P2 = 1 << 5;
constexpr u16 PF2P1 = 1 << 4;
constexpr u16 PF2P0 = 1 << 3;
constexpr u16 PF1P2 = 1 << 2;
constexpr u16 PF1P1 = 1 << 1;
constexpr u16 PF1P0 = 1 << 0;
} // namespace BPLCON2

// http://www.winnicki.net/amiga/memmap/BPLCON3.html
namespace BPLCON3 {
constexpr u16 BANK2 = 1 << 15;
constexpr u16 BANK1 = 1 << 14;
constexpr u16 BANK0 = 1 << 13;
constexpr u16 PF2OF2 = 1 << 12;
constexpr u16 PF2OF1 = 1 << 11;
constexpr u16 PF2OF0 = 1 << 10;
constexpr u16 LOCT = 1 << 9;
constexpr u16 SPRES1 = 1 << 7;
constexpr u16 SPRES0 = 1 << 6;
constexpr u16 BRDRBLNK = 1 << 5;
constexpr u16 BRDNTRAN = 1 << 4;
constexpr u16 ZDCLKEN = 1 << 2;
constexpr u16 BRDSPRT = 1 << 1;
constexpr u16 EXTBLKEN = 1 << 0;

constexpr u16 BANK(u16 bank) {
  return ((bank & 1) ? BANK0 : 0) | ((bank & 2) ? BANK1 : 0) |
         ((bank & 4) ? BANK2 : 0);
}

constexpr u16 PF2OF(u16 offset) {
  return ((offset & 1) ? PF2OF0 : 0) | ((offset & 2) ? PF2OF1 : 0) |
         ((offset & 4) ? PF2OF2 : 0);
}
} // namespace BPLCON3

// http://www.winnicki.net/amiga/memmap/FMODE.html
namespace FMODE {
constexpr u16 SSCAN2 = 1 << 15;
constexpr u16 BSCAN2 = 1 << 14;
constexpr u16 SPAGEM = 1 << 3;
constexpr u16 SPR32 = 1 << 2;
constexpr u16 BPAGEM = 1 << 1;
constexpr u16 BLP32 = 1 << 0;
} // namespace FMODE

inline volatile Custom &custom =
    *reinterpret_cast<volatile Custom *const>(0xdff000UL);
