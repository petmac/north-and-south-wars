#include "game.h"

Game initGame() {
  return (Game){
      .state = GameState::loadingFontAndPalette,
  };
}

void updateGame(Game &game) {
  switch (game.state) {
  case GameState::loadingFontAndPalette:
    break;
  }
}
