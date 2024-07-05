#pragma once

#include "frame_chip.h" // Background

struct FrameFast;
struct Intro;

void drawIntro(Background &background, FrameFast &frameFast,
               const Intro &intro);
