#include "amiga/libs.h"

#include <proto/exec.h>

ExecBase *SysBase;
DosLibrary *DOSBase;
struct GfxBase *GfxBase;

bool loadLibs() {
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Warray-bounds"
  SysBase = *reinterpret_cast<ExecBase *const *>(4UL);
#pragma GCC diagnostic pop
  GfxBase =
      reinterpret_cast<struct GfxBase *>(OpenLibrary("graphics.library", 0));
  if (GfxBase == nullptr) {
    return false;
  }

  DOSBase = reinterpret_cast<DosLibrary *>(OpenLibrary("dos.library", 0));
  if (DOSBase == nullptr) {
    CloseLibrary(reinterpret_cast<Library *>(GfxBase));

    return false;
  }

  return true;
}

void unloadLibs() {
  CloseLibrary(reinterpret_cast<Library *>(DOSBase));
  CloseLibrary(reinterpret_cast<Library *>(GfxBase));
}
