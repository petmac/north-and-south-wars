#include "draw.h"

#include "blit.h"
#include "chip.h"
#include "draw_intro.h"
#include "frame_fast.h"
#include "text.h"

#include "game/game.h"

static void drawLoadingIntro(Background &background, FrameFast &frameFast) {
  if (frameFast.state == FrameState::drawnLoading) {
    return;
  }

  clear(background);
  drawText(background, chip.smallFont, 0, 0, "Loading intro");
  frameFast.state = FrameState::drawnLoading;
}

static void drawError(Background &background, FrameFast &frameFast) {
  if (frameFast.state == FrameState::drawnError) {
    return;
  }

  clear(background);
  drawText(background, chip.smallFont, 0, 0, "Error");
  frameFast.state = FrameState::drawnError;
}

void drawGame(FrameChip &frameChip, FrameFast &frameFast, const Game &game) {
  Background &background = frameChip.background;
  switch (game.state) {
  case GameState::loadingFontAndPalette:
    break;
  case GameState::loadingIntro:
    drawLoadingIntro(background, frameFast);
    break;
  case GameState::intro:
  case GameState::loadingTitle:
    drawIntro(background, frameFast, game.intro);
    break;
  case GameState::error:
    drawError(background, frameFast);
    break;
  }
}
