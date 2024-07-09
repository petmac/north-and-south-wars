#pragma once

#include "frame_chip.h" // Background

struct FrameFast;
struct Mission;

void drawMission(Background &background, FrameFast &frameFast,
                 const Mission &mission);
