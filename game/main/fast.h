#pragma once

#include "frame_fast.h"

#include "game/game.h"

struct Fast {
  Game game;
  u8 backBufferIndex;
  FrameFast frames[2];
};

void initFast(Fast &fast);
