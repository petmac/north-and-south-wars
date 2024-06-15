#include "libs.h"

#include <proto/exec.h>

ExecBase *SysBase;
DosLibrary *DOSBase;
struct GfxBase *GfxBase;

bool loadLibs() {
  SysBase = *reinterpret_cast<ExecBase *const *const>(4UL);
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
