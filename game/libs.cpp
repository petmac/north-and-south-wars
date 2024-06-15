#include "libs.h"

#include <proto/exec.h>

ExecBase *SysBase;
DosLibrary *DOSBase;
struct GfxBase *GfxBase;

bool loadLibs() {
  SysBase = *((struct ExecBase **)4UL);
  GfxBase = (struct GfxBase *)OpenLibrary((CONST_STRPTR) "graphics.library", 0);
  if (!GfxBase) {
    return false;
  }

  DOSBase = (struct DosLibrary *)OpenLibrary((CONST_STRPTR) "dos.library", 0);
  if (!DOSBase) {
    CloseLibrary((struct Library *)GfxBase);

    return false;
  }

  return true;
}

void unloadLibs() {
  CloseLibrary((struct Library *)DOSBase);
  CloseLibrary((struct Library *)GfxBase);
}
