#include "game/game.h"

#include "intro.h"
#include "title.h"

#include "game/callbacks.h"

Game initGame() {
  return (Game){
      .state = GameState::loadingFontAndPalette,
      .intro = {},
      .title = {},
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
    updateIntro(game.intro, game);
    break;

  case GameState::loadingTitle:
    game.state = GameState::title;
    break;

  case GameState::title:
    updateTitle(game.title, game);
    break;

  case GameState::loadingCampaignMap:
    break;

  case GameState::error:
    break;
  }
}

// Intro callbacks
void goToTitleScreen(Game &game) { game.state = GameState::loadingTitle; }

// Title callbacks
void goToCampaignMap(Game &game) { game.state = GameState::loadingCampaignMap; }
