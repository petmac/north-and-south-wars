#pragma once

#include "campaign_map.h"
#include "intro.h"
#include "mission/mission.h"
#include "mission_lost.h"
#include "mission_won.h"
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
  missionLost,
  missionWon,
  error,
};

struct Game {
  GameState state;
  Intro intro;
  Title title;
  CampaignMap campaignMap;
  Mission mission;
  MissionLost missionLost;
  MissionWon missionWon;
};

void initGame(Game &game);
void updateGame(Game &game, u16 mouseX, u16 mouseY);
void mouseClicked(Game &game, u16 mouseX, u16 mouseY);
void mouseRightClicked(Game &game);
