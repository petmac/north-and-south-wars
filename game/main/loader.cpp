#include "game/callbacks.h"

#include "assets/palette.h"
#include "chip.h"

#include "gcc8_c_support.h"

#include <proto/dos.h>

template <typename T> static bool load(T &dst, const char *path) {
  constexpr u16 size = sizeof(T);

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

bool loadMap(Map &map) { return load(map, "data/mission/map.map"); }

bool loadMissionAssets() {
  MissionChip &mc = chip.mission;

  return load(mc.attack.backgrounds.bridge,
              "data/mission/attack/bg_bridge.BPL") &&
         load(mc.attack.backgrounds.mountain,
              "data/mission/attack/bg_mountain.BPL") &&
         load(mc.attack.backgrounds.plain,
              "data/mission/attack/bg_plain.BPL") &&
         load(mc.attack.backgrounds.road, "data/mission/attack/bg_road.BPL") &&
         load(mc.attack.backgrounds.woods,
              "data/mission/attack/bg_woods.BPL") &&
         load(mc.attack.units, "data/mission/attack/units.BPL") &&
         load(mc.arrows, "data/mission/arrows.BPL") &&
         load(mc.menu, "data/mission/menu.BPL") &&
         load(mc.tileset, "data/mission/tiles.bpl") &&
         load(mc.units, "data/mission/units.BPL");
}

bool loadMousePointer() { return load(chip.mousePointer, "data/mouse.SPR"); }

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

bool loadSmallFont() { return load(chip.smallFont, "data/small_font.BPL"); }
