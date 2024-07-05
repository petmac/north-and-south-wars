#pragma once

#include "intro.h"

enum class GameState : u8 {
  loadingFontAndPalette,
  loadingIntro,
  intro,
  loadingTitle,
  error,
};

struct Game {
  GameState state;
  Intro intro;
};

Game initGame();
void updateGame(Game &game);
