#pragma once

struct Map;

bool loadMap(Map &map);
bool loadMissionAssets();
bool loadMousePointer();
bool loadPalette();
bool loadSmallFont();

// TODO Horrendous debugging hack
extern "C" void KPrintF(const char *fmt, ...);
