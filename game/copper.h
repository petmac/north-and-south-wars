#pragma once

#include <exec/types.h>

extern UWORD copper1[];
extern UWORD copper2[];

USHORT *screenScanDefault(USHORT *copListEnd);
USHORT *copSetPlanes(UBYTE bplPtrStart, USHORT *copListEnd,
                     const UBYTE **planes, int numPlanes);
USHORT *copSetColor(USHORT *copListCurrent, USHORT index, USHORT color);
