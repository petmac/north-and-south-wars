#pragma once

#include "frame_fast.h"

#include "game/game.h"

struct Fast {
  Game game;
  u8 backBufferIndex;
  FrameFast frames[2];
  u8 lastMouseDataX;
  u8 lastMouseDataY;
  u16 mouseX;
  u16 mouseY;
};

void initFast(Fast &fast);
