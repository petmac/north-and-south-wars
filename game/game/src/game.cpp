#include "game/game.h"

#include "game/callbacks.h"

Game initGame() {
  return (Game){
      .state = GameState::loadingFontAndPalette,
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
    break;

  case GameState::intro:
    break;

  case GameState::error:
    break;
  }
}
