#include "draw.h"

#include "blit.h"
#include "chip.h"
#include "draw_campaign_map.h"
#include "draw_intro.h"
#include "draw_mission.h"
#include "draw_mission_lost.h"
#include "draw_mission_won.h"
#include "draw_title.h"
#include "frame_fast.h"
#include "text.h"

#include "game/game.h"

static constexpr u16 centerY = (256 - 8) / 2;

static void drawLoading(Background &background, FrameFast &frameFast) {
  constexpr u16 x = (40 - 7) / 2;

  if (frameFast.state == FrameState::drawnLoading) {
    return;
  }

  clear(background);
  drawText(background, chip.main.smallFont, x, centerY, "Loading");
  frameFast.state = FrameState::drawnLoading;
}

static void drawError(Background &background, FrameFast &frameFast) {
  constexpr u16 x = (40 - 5) / 2;

  if (frameFast.state == FrameState::drawnError) {
    return;
  }

  clear(background);
  drawText(background, chip.main.smallFont, x, centerY, "Error");
  frameFast.state = FrameState::drawnError;
}

void drawGame(FrameChip &frameChip, FrameFast &frameFast, const Game &game) {
  Background &background = frameChip.background;
  switch (game.state) {
  case GameState::loadingFontAndPalette:
    break;
  case GameState::loadingIntro:
  case GameState::loadingTitle:
  case GameState::loadingCampaignMap:
  case GameState::loadingMission:
    drawLoading(background, frameFast);
    break;
  case GameState::intro:
    drawIntro(background, frameFast, game.intro);
    break;
  case GameState::title:
    drawTitle(background, frameFast, game.title);
    break;
  case GameState::campaignMap:
    drawCampaignMap(background, frameFast, game.campaignMap, game.nextMission);
    break;
  case GameState::playingMission:
    drawMission(background, frameFast, game.mission);
    break;
  case GameState::missionLost:
    drawMissionLost(background, frameFast, game.missionLost);
    break;
  case GameState::missionWon:
    drawMissionWon(background, frameFast, game.missionWon);
    break;
  case GameState::error:
    drawError(background, frameFast);
    break;
  }
}
