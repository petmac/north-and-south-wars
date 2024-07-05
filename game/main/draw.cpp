#include "draw.h"

#include "blit.h"
#include "chip.h"
#include "frame_fast.h"
#include "text.h"

#include "game/game.h"
#include "game/intro.h"

static void drawLoadingIntro(Background &background, FrameFast &frameFast) {
  if (frameFast.state == FrameState::drawnLoading) {
    return;
  }

  clear(background);
  drawText(background, chip.smallFont, 0, 0, "Loading intro");
  frameFast.state = FrameState::drawnLoading;
}

static void drawIntro(Background &background, FrameFast &frameFast,
                      const Intro &intro) {
  // Clear if needed.
  switch (frameFast.state) {
  case FrameState::cleared:
  case FrameState::drawnIntro:
    break;
  default:
    clear(background);
    break;
  }

  // Need to complete partial lines?
  while (frameFast.intro.linesDrawn < intro.linesComplete) {
    const char *const line = introTextLines[frameFast.intro.linesDrawn];
    const char *const str = &line[frameFast.intro.charsDrawn];

    drawText(background, chip.smallFont, frameFast.intro.charsDrawn,
             frameFast.intro.linesDrawn * 8, str);

    ++frameFast.intro.linesDrawn;
    frameFast.intro.charsDrawn = 0;
  }

  // Need to draw characters?
  while (frameFast.intro.charsDrawn < intro.charsComplete) {
    const char *const line = introTextLines[frameFast.intro.linesDrawn];
    const char ch = line[frameFast.intro.charsDrawn];
    const char str[2] = {ch, '\0'};

    drawText(background, chip.smallFont, frameFast.intro.charsDrawn,
             frameFast.intro.linesDrawn * 8, str);

    ++frameFast.intro.charsDrawn;
  }

  frameFast.state = FrameState::drawnIntro;
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
    drawIntro(background, frameFast, game.intro);
    break;
  case GameState::error:
    drawError(background, frameFast);
    break;
  }
}
