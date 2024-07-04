#pragma once

#include "util/types.h"

enum class GameState : u8 {
  loadingFontAndPalette,
  loadingIntro,
  intro,
  error,
};

struct Game {
  GameState state;
};

Game initGame();
void updateGame(Game &game);
