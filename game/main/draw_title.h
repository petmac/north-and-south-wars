#pragma once

#include "frame_chip.h" // Background

struct FrameFast;
struct Title;

void drawTitle(Background &background, FrameFast &frameFast,
               const Title &title);
