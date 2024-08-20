#include "amiga/mouse.h"

#include "amiga/cia.h"
#include "amiga/custom.h"

bool mouseLeft() { return (ciaa.ciapra & CIAF_GAMEPORT0) == 0; }

bool mouseRight() { return (custom.potinp & POTGO::DATLY) == 0; }
