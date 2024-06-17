#include "copper.h"
#include "libs.h"
#include "mouse.h"
#include "system.h"

#include <hardware/dmabits.h>
#include <hardware/intbits.h>
#include <proto/dos.h>

static __attribute__((interrupt)) void interruptHandler() {
  custom.intreq = (1 << INTB_VERTB);
  custom.intreq = (1 << INTB_VERTB); // reset vbl req. twice for a4000 bug.
}

int main() {
  if (!loadLibs()) {
    Exit(0);
  }

  TakeSystem();

  // Start with display disabled
  custom.cop1lc = 0;
  custom.cop2lc = 0;
  custom.dmacon = DMAF_BLITTER; // disable blitter dma for copjmp bug
  custom.copjmp1 = 0x7fff;      // start coppper
  custom.dmacon = DMAF_SETCLR | DMAF_MASTER | DMAF_BLITTER;

  SetInterruptHandler(interruptHandler);

  custom.intena = INTF_SETCLR | INTF_INTEN | INTF_VERTB;
  custom.intreq = INTF_VERTB; // reset vbl req

  while (!MouseLeft()) {
  }

  // END
  FreeSystem();
  unloadLibs();
}
