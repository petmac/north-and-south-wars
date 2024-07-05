#pragma once

#include "intro.h"
#include "title.h"

enum class GameState : u8 {
  loadingFontAndPalette,
  loadingIntro,
  intro,
  loadingTitle,
  title,
  error,
};

struct Game {
  GameState state;
  Intro intro;
  Title title;
};

Game initGame();
void updateGame(Game &game);
