#pragma once

#include "campaign_map.h"
#include "intro.h"
#include "mission/mission.h"
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

void initGame(Game &game);
void updateGame(Game &game, u16 mouseX, u16 mouseY);
void mouseClicked(Game &game, u16 mouseX, u16 mouseY);
void mouseRightClicked(Game &game);
