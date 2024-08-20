#pragma once

#include "frame_fast.h"

#include "game/game.h"

#include <exec/interrupts.h> // Interrupt

struct Fast {
  // Written on main thread
  Game game;
  u8 backBufferIndex;
  FrameFast frames[2];
  Interrupt interrupt;

  // Written during vertical blank interrupt
  u16 mouseX;
  u16 mouseY;
  bool mouseClicked;
  bool mouseRightClicked;

  // Read and written during vertical blank interrupt
  u8 lastMouseDataX;
  u8 lastMouseDataY;
  bool lastMouseLeftButton;
  bool lastMouseRightButton;
};

void initFast(Fast &fast);
