#include "draw_intro.h"

#include "blit.h"
#include "chip.h"
#include "frame_fast.h"
#include "text.h"

#include "game/intro.h"

void drawIntro(Background &background, FrameFast &frameFast,
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

    drawText(background, chip.main.smallFont, frameFast.intro.charsDrawn,
             frameFast.intro.linesDrawn * 8, str);

    ++frameFast.intro.linesDrawn;
    frameFast.intro.charsDrawn = 0;
  }

  // Need to draw characters?
  while (frameFast.intro.charsDrawn < intro.charsComplete) {
    const char *const line = introTextLines[frameFast.intro.linesDrawn];
    const char ch = line[frameFast.intro.charsDrawn];
    const char str[2] = {ch, '\0'};

    drawText(background, chip.main.smallFont, frameFast.intro.charsDrawn,
             frameFast.intro.linesDrawn * 8, str);

    ++frameFast.intro.charsDrawn;
  }

  frameFast.state = FrameState::drawnIntro;
}
