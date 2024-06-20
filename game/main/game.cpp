#include "game.h"

#include "loader.h"

Game initGame() {
  return (Game){
      .state = GameState::loadingFontAndPalette,
  };
}

void updateGame(Game &game) {
  switch (game.state) {
  case GameState::loadingFontAndPalette:
    if (!load(Resource::smallFont) || !load(Resource::palette)) {
      return;
    }
    game.state = GameState::loadingIntro;
    break;

  case GameState::loadingIntro:
    break;
  }
}
