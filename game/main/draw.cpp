#include "draw.h"

#include "chip.h"
#include "text.h"

#include "game/game.h"

static void drawLoadingIntro(Background &background) {
  drawText(background, chip.smallFont, 0, 0, "Loading intro");
}

static void drawError(Background &background) {
  drawText(background, chip.smallFont, 0, 0, "Error");
}

void draw(FrameChip &frameChip, FrameFast &frameFast, const Game &game) {
  Background &background = frameChip.background;
  switch (game.state) {
  case GameState::loadingFontAndPalette:
    break;
  case GameState::loadingIntro:
    drawLoadingIntro(background);
    break;
  case GameState::error:
    drawError(background);
    break;
  }
}
