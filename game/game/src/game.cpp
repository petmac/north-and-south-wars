#include "game/game.h"

#include "campaign_map.h"
#include "intro.h"
#include "mission/mission.h"
#include "title.h"

#include "game/callbacks.h"

void initGame(Game &game) { game.state = GameState::loadingFontAndPalette; }

void updateGame(Game &game, u16 mouseX, u16 mouseY) {
  switch (game.state) {
  case GameState::loadingFontAndPalette:
    if (!loadMainAssets() || !loadPalette()) {
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
    if (!loadMap(game.mission.map) || !loadMissionAssets()) {
      game.state = GameState::error;
      return;
    }
    game.state = GameState::playingMission;
    startMission(game.mission);
    break;

  case GameState::playingMission:
    updateMission(game.mission, mouseX, mouseY);
    break;

  case GameState::error:
    break;
  }
}

void mouseClicked(Game &game, u16 mouseX, u16 mouseY) {
  switch (game.state) {
  case GameState::loadingFontAndPalette:
  case GameState::loadingIntro:
    break;

  case GameState::intro:
    introMouseClicked(game);
    break;

  case GameState::loadingTitle:
    break;

  case GameState::title:
    titleMouseClicked(game);
    break;

  case GameState::loadingCampaignMap:
  case GameState::campaignMap:
  case GameState::loadingMission:
    break;

  case GameState::playingMission:
    missionMouseClicked(game.mission, mouseX, mouseY);
    break;

  case GameState::error:
    break;
  }
}

void mouseRightClicked(Game &game) {
  switch (game.state) {
  case GameState::loadingFontAndPalette:
  case GameState::loadingIntro:
  case GameState::intro:
  case GameState::loadingTitle:
  case GameState::title:
  case GameState::loadingCampaignMap:
  case GameState::campaignMap:
  case GameState::loadingMission:
    break;

  case GameState::playingMission:
    missionMouseRightClicked(game.mission);
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
