#include "amiga/mouse.h"

#include "util/types.h"

bool mouseLeft() { return !((*(volatile u8 *)0xbfe001) & 64); }
