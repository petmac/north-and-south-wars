#include "game/game.h"

#include "intro.h"

#include "game/callbacks.h"

Game initGame() {
  return (Game){
      .state = GameState::loadingFontAndPalette,
      .intro = {},
  };
}

void updateGame(Game &game) {
  switch (game.state) {
  case GameState::loadingFontAndPalette:
    if (!loadSmallFont() || !loadPalette()) {
      game.state = GameState::error;
      return;
    }
    game.state = GameState::loadingIntro;
    break;

  case GameState::loadingIntro:
    game.state = GameState::intro;
    game.intro.linesComplete = 0;
    game.intro.charsComplete = 0;
    break;

  case GameState::intro:
    updateIntro(game.intro);
    break;

  case GameState::error:
    break;
  }
}
