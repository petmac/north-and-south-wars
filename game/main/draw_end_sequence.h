#pragma once

#include "background.h"

struct FrameFast;
struct EndSequence;

void drawEndSequence(Background &background, FrameFast &frameFast,
                     const EndSequence &endSequence);
