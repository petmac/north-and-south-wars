#include "amiga/mouse.h"

#include "amiga/cia.h"

bool mouseLeft() { return (ciaa.ciapra & CIAF_GAMEPORT0) == 0; }
