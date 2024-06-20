#include "amiga/mouse.h"

#include "util/types.h"

bool MouseLeft() { return !((*(volatile u8 *)0xbfe001) & 64); }
