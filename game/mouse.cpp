#include "mouse.h"

#include "types.h"

bool MouseLeft() { return !((*(volatile u8 *)0xbfe001) & 64); }
