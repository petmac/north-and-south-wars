#include "amiga/system.h"

#include "amiga/custom.h"
#include "gcc8_c_support.h"

#include <graphics/gfxbase.h>
#include <proto/exec.h>
#include <proto/graphics.h>

// backup
static View *ActiView;

void TakeSystem() {
  Forbid();

  ActiView = GfxBase->ActiView; // store current view

  LoadView(0);
  WaitTOF();
  WaitTOF();

  WaitVbl();
  WaitVbl();

  OwnBlitter();
  WaitBlit();

  // set all colors black
  for (int a = 0; a < 32; a++) {
    custom.color[a] = 0;
  }

  WaitVbl();
  WaitVbl();

  WaitVbl();
}

void FreeSystem() {
  WaitVbl();
  WaitBlit();

  /*Restore system copper list(s). */
  custom.cop1lc = (u32)GfxBase->copinit;
  custom.cop2lc = (u32)GfxBase->LOFlist;
  custom.copjmp1 = 0x7fff; // start coppper

  WaitBlit();
  DisownBlitter();

  LoadView(ActiView);
  ActiView = nullptr;

  WaitTOF();
  WaitTOF();

  Permit();
}

// vblank begins at vpos 312 hpos 1 and ends at vpos 25 hpos 1
// vsync begins at line 2 hpos 132 and ends at vpos 5 hpos 18
void WaitVbl() {
  debug_start_idle();
  while (1) {
    u32 vpos = custom.vpos32;
    vpos &= 0x1ff00;
    if (vpos != (311 << 8))
      break;
  }
  while (1) {
    u32 vpos = custom.vpos32;
    vpos &= 0x1ff00;
    if (vpos == (311 << 8))
      break;
  }
  debug_stop_idle();
}
