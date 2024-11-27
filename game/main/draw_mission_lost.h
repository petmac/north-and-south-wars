#pragma once

#include "background.h"

struct FrameFast;
struct MissionLost;

void drawMissionLost(Background &background, FrameFast &frameFast,
                     const MissionLost &missionLost);
