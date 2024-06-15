#include "mouse.h"

#include <exec/types.h>

bool MouseLeft() { return !((*(volatile UBYTE *)0xbfe001) & 64); }
