#include "game/game.h"

#include "game/loader.h"

Game initGame() {
  return (Game){
      .state = GameState::loadingFontAndPalette,
  };
}

void updateGame(Game &game) {
  switch (game.state) {
  case GameState::loadingFontAndPalette:
    if (!loadSmallFont() || !loadPalette()) {
      return;
    }
    game.state = GameState::loadingIntro;
    break;

  case GameState::loadingIntro:
    break;
  }
}
