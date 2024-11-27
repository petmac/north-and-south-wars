#pragma once

#include "background.h"

struct FrameFast;
struct MissionWon;

void drawMissionWon(Background &background, FrameFast &frameFast,
                    const MissionWon &missionWon);
