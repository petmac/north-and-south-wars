#pragma once

#include "background.h"

struct FrameFast;
struct Mission;

void drawMissionAttack(Background &background, FrameFast &frameFast,
                       const Mission &mission);
