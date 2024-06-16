#pragma once

#include "types.h"

extern u16 copper1[];
extern u16 copper2[];

u16 *screenScanDefault(u16 *copListEnd);
u16 *copSetPlanes(u8 bplPtrStart, u16 *copListEnd, const u8 **planes,
                  int numPlanes);
u16 *copSetColor(u16 *copListCurrent, u16 index, u16 color);
