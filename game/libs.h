#pragma once

struct DosLibrary;
struct ExecBase;
struct GfxBase;

extern ExecBase *SysBase;
extern DosLibrary *DOSBase;
extern struct GfxBase *GfxBase;

bool loadLibs();
void unloadLibs();
