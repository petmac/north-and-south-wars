#pragma once

#include "campaign_map.h"
#include "intro.h"
#include "mission.h"
#include "title.h"

enum class GameState : u8 {
  loadingFontAndPalette,
  loadingIntro,
  intro,
  loadingTitle,
  title,
  loadingCampaignMap,
  campaignMap,
  loadingMission,
  playingMission,
  error,
};

struct Game {
  GameState state;
  Intro intro;
  Title title;
  CampaignMap campaignMap;
  Mission mission;
};

Game initGame();
void updateGame(Game &game);
void mouseClicked(Game &game, u16 mouseX, u16 mouseY);
