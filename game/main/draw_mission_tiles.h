#pragma once

#include "background.h"

struct FrameFast;
struct Mission;

void drawMissionTiles(Background &background, FrameFast &frameFast,
                      const Mission &mission);
