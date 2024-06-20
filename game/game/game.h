#pragma once

#include "types.h"

enum class GameState : u8 {
  loadingFontAndPalette,
  loadingIntro,
};

struct Game {
  GameState state;
};

Game initGame();
void updateGame(Game &game);
