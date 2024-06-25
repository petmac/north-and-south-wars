#include "game/callbacks.h"

#include "chip.h"

#include "gcc8_c_support.h"

#include <proto/dos.h>

static bool load(void *dst, u16 size, const char *path) {
  KPrintF("Loading \"%s\"", reinterpret_cast<u32>(path));
  KPrintF("Size = %lu", size);
  u32 file = Open(path, MODE_OLDFILE);
  KPrintF("Open result = %lu", file);
  if (file == 0) {
    KPrintF("Could not open file");
    return false;
  }

  const u32 bytesRead = Read(file, dst, size);
  KPrintF("Read %lu bytes", bytesRead);
  if (bytesRead != size) {
    KPrintF("Bytes read %lu does not match buffer size %lu", bytesRead, size);
  }

  Close(file);
  file = 0;

  return true;
}

bool loadSmallFont() {
  return load(&chip.smallFont, sizeof(chip.smallFont), "data/small_font.BPL");
}

bool loadPalette() { return true; }
