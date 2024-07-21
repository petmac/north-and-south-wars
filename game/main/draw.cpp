#include "draw.h"

#include "blit.h"
#include "chip.h"
#include "draw_campaign_map.h"
#include "draw_intro.h"
#include "draw_mission.h"
#include "draw_title.h"
#include "frame_fast.h"
#include "text.h"

#include "game/game.h"

static constexpr u16 centerY = (256 - 8) / 2;

static void updateMousePointerPosition() {
  const u16 hStart = 128;
  const u16 vStart = 44;
  const u16 vStop = vStart + MousePointerSpriteImage::height;

  chip.mousePointer.position = spritePosition(vStart, hStart);
  chip.mousePointer.control = spriteControl(vStart, vStop, hStart);
}

static void drawLoading(Background &background, FrameFast &frameFast) {
  constexpr u16 x = (40 - 7) / 2;

  if (frameFast.state == FrameState::drawnLoading) {
    return;
  }

  clear(background);
  drawText(background, chip.smallFont, x, centerY, "Loading");
  frameFast.state = FrameState::drawnLoading;
}

static void drawError(Background &background, FrameFast &frameFast) {
  constexpr u16 x = (40 - 5) / 2;

  if (frameFast.state == FrameState::drawnError) {
    return;
  }

  clear(background);
  drawText(background, chip.smallFont, x, centerY, "Error");
  frameFast.state = FrameState::drawnError;
}

void drawGame(FrameChip &frameChip, FrameFast &frameFast, const Game &game) {
  updateMousePointerPosition();

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
    drawCampaignMap(background, frameFast, game.campaignMap);
    break;
  case GameState::playingMission:
    drawMission(background, frameFast, game.mission);
    break;
  case GameState::error:
    drawError(background, frameFast);
    break;
  }
}
