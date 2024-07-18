#pragma once

#include "background.h"

struct FrameFast;
struct Mission;

void drawMission(Background &background, FrameFast &frameFast,
                 const Mission &mission);
