#include "game/game.h"

#include "campaign_map.h"
#include "intro.h"
#include "mission.h"
#include "title.h"

#include "game/callbacks.h"

Game initGame() {
  return (Game){
      .state = GameState::loadingFontAndPalette,
      .intro = {},
      .title = {},
      .campaignMap = {},
      .mission = {},
  };
}

void updateGame(Game &game) {
  switch (game.state) {
  case GameState::loadingFontAndPalette:
    if (!loadMousePointer() || !loadPalette() || !loadSmallFont()) {
      game.state = GameState::error;
      return;
    }
    game.state = GameState::loadingIntro;
    break;

  case GameState::loadingIntro:
    if (false) {
      game.state = GameState::error;
      return;
    }
    game.state = GameState::intro;
    startIntro(game.intro);
    break;

  case GameState::intro:
    updateIntro(game.intro, game);
    break;

  case GameState::loadingTitle:
    if (false) {
      game.state = GameState::error;
      return;
    }
    game.state = GameState::title;
    startTitle(game.title);
    break;

  case GameState::title:
    updateTitle(game.title, game);
    break;

  case GameState::loadingCampaignMap:
    if (false) {
      game.state = GameState::error;
      return;
    }
    game.state = GameState::campaignMap;
    startCampaignMap(game.campaignMap);
    break;

  case GameState::campaignMap:
    updateCampaignMap(game.campaignMap, game);
    break;

  case GameState::loadingMission:
    if (!loadTileset() || !loadMap(game.mission.map) || !loadUnits()) {
      game.state = GameState::error;
      return;
    }
    game.state = GameState::playingMission;
    startMission(game.mission);
    break;

  case GameState::playingMission:
    updateMission(game.mission, game);
    break;

  case GameState::error:
    break;
  }
}

// Intro callbacks
void loadTitleScreen(Game &game) { game.state = GameState::loadingTitle; }

// Title callbacks
void loadCampaignMap(Game &game) { game.state = GameState::loadingCampaignMap; }

// Campaign map callbacks
void loadMission(Game &game) { game.state = GameState::loadingMission; }
