#include "game/callbacks.h"

#include "assets/palette.h"
#include "chip.h"

#include "amiga/doynax.h"
#include "gcc8_c_support.h"

#include <proto/dos.h>

static u8 unpackBuffer[16 * 1024];

template <typename T> static bool load(T &dst, const char *path) {
  constexpr u32 size = sizeof(T);

  KPrintF("Loading \"%s\"", reinterpret_cast<u32>(path));
  KPrintF("Size = %ld", size);
  u32 file = Open(path, MODE_OLDFILE);
  KPrintF("Open result = %ld", file);
  if (file == 0) {
    KPrintF("Could not open file");
    return false;
  }

  const u32 bytesRead = Read(file, &dst, size);
  KPrintF("Read %ld bytes", bytesRead);
  if (bytesRead != size) {
    KPrintF("Bytes read %ld does not match buffer size %ld", bytesRead, size);
  }

  Close(file);
  file = 0;

  return true;
}

template <typename T> static bool loadAndUnpack(T &dst, const char *path) {
  if (!load(unpackBuffer, path)) {
    return false;
  }

  void *const end = doynaxdepack(unpackBuffer, &dst);
  const u32 unpackedDataSize =
      static_cast<const u8 *>(end) - reinterpret_cast<const u8 *>(&dst);

  if (unpackedDataSize != sizeof(dst)) {
    KPrintF("Unpacked data size %ld does not match destination size %ld",
            unpackedDataSize, sizeof(dst));
  }

  return true;
}

bool loadCampaignAssets() {
  return loadAndUnpack(chip.campaign, "data/campaign.chip.lz");
}

bool loadMap(Map &map) { return load(map, "data/mission/map.map"); }

bool loadMainAssets() { return loadAndUnpack(chip.main, "data/main.chip.lz"); }

bool loadMissionAssets() {
  return loadAndUnpack(chip.mission, "data/mission.chip.lz");
}

bool loadPalette() {
  Palette palette;
  if (!load(palette, "data/palette.PAL")) {
    return false;
  }

  for (u16 colorIndex = 0; colorIndex < 32; ++colorIndex) {
    chip.frames[0].copper.colors[colorIndex].value = palette[colorIndex];
    chip.frames[1].copper.colors[colorIndex].value = palette[colorIndex];
  }

  return true;
}

bool loadTitleAssets() {
  return loadAndUnpack(chip.title, "data/title.chip.lz");
}
