#pragma once

#include "background.h"

struct FrameFast;
struct Intro;

void drawIntro(Background &background, FrameFast &frameFast,
               const Intro &intro);
