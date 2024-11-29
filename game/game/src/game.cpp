#include "game/game.h"

#include "campaign_map.h"
#include "end_sequence.h"
#include "intro.h"
#include "mission/mission.h"
#include "mission_lost.h"
#include "mission_won.h"
#include "title.h"

#include "game/callbacks.h"
#include "game/missions.h"

void initGame(Game &game) {
  game.state = GameState::loadingFontAndPalette;
  game.nextMission = 0;
}

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
    if (!loadTitleAssets()) {
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
    if (!loadCampaignAssets()) {
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
    if (!loadMap(game.mission.map, game.nextMission) || !loadMissionAssets()) {
      game.state = GameState::error;
      return;
    }
    game.state = GameState::playingMission;
    startMission(game.mission);
    break;

  case GameState::playingMission:
    updateMission(game.mission, mouseX, mouseY, game);
    break;

  case GameState::missionLost:
    updateMissionLost(game.missionLost);
    break;

  case GameState::missionWon:
    updateMissionWon(game.missionWon);
    break;

  case GameState::endSequence:
    updateEndSequence(game.endSequence);
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
    break;

  case GameState::campaignMap:
    campaignMouseClicked(game);
    break;

  case GameState::loadingMission:
    break;

  case GameState::playingMission:
    missionMouseClicked(game.mission, mouseX, mouseY);
    break;

  case GameState::missionLost:
    missionLostMouseClicked(game);
    break;

  case GameState::missionWon:
    missionWonMouseClicked(game);
    break;

  case GameState::endSequence:
    endSequenceMouseClicked(game);
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

  case GameState::missionLost:
  case GameState::missionWon:
  case GameState::endSequence:
  case GameState::error:
    break;
  }
}

// Intro callbacks
void loadTitleScreen(Game &game) { game.state = GameState::loadingTitle; }

// Title callbacks
// Mission lost callbacks
void loadCampaignMap(Game &game) { game.state = GameState::loadingCampaignMap; }

// Campaign map callbacks
void loadMission(Game &game) { game.state = GameState::loadingMission; }

// Mission callbacks
void missionLost(Game &game) {
  game.state = GameState::missionLost;
  startMissionLost(game.missionLost);
}
void missionWon(Game &game) {
  game.state = GameState::missionWon;
  startMissionWon(game.missionWon);
}

// Mission won callbacks
void missionWonFinished(Game &game) {
  const u16 nextMission = game.nextMission + 1;
  if (nextMission >= missionCount) {
    game.nextMission = 0;
    game.state = GameState::endSequence;
    startEndSequence(game.endSequence);
  } else {
    // Proceed to the next mission
    game.nextMission = nextMission;
    loadCampaignMap(game);
  }
}

// End sequence callbacks
void endSequenceFinished(Game &game) { loadTitleScreen(game); }
