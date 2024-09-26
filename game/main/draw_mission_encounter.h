#pragma once

#include "background.h"

struct FrameFast;
struct Mission;

void drawMissionEncounter(Background &background, FrameFast &frameFast,
                          const Mission &mission);
