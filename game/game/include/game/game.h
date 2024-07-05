#pragma once

#include "campaign_map.h"
#include "intro.h"
#include "title.h"

enum class GameState : u8 {
  loadingFontAndPalette,
  loadingIntro,
  intro,
  loadingTitle,
  title,
  loadingCampaignMap,
  campaignMap,
  error,
};

struct Game {
  GameState state;
  Intro intro;
  Title title;
  CampaignMap campaignMap;
};

Game initGame();
void updateGame(Game &game);
