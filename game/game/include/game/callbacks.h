#pragma once

#include "util/types.h"

enum class Sound : u8;
struct Map;

bool loadCampaignAssets();
bool loadMap(Map &map, u16 missionIndex);
bool loadMainAssets();
bool loadMissionAssets();
bool loadPalette();
bool loadTitleAssets();
void play(Sound sound);

// TODO Horrendous debugging hack
extern "C" void KPrintF(const char *fmt, ...);
