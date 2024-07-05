#include "intro.h"

#include "game/callbacks.h"
#include "game/intro.h"

const char *const introTextLines[] = {
    // 0000000111111111122222222223333333333
    // 3456789012345678901234567890123456789
    "Lorem ipsum dolor sit amet, consectetur",
    "adipiscing elit, sed do eiusmod tempor",
    "incididunt ut labore et dolore magna",
    "",
    "aliqua. Ut enim ad minim veniam, quis",
    "nostrud exercitation ullamco laboris",
    "nisi ut aliquip ex ea commodo",
    "",
    "consequat. Duis aute irure dolor in",
    "reprehenderit in voluptate velit esse",
    "cillum dolore eu fugiat nulla pariatur.",
    "",
    "Excepteur sint occaecat cupidatat non",
    "proident, sunt in culpa qui officia",
    "deserunt mollit anim id est laborum.",
};

void updateIntro(Intro &intro, Game &game) {
  constexpr u16 lineCount = sizeof(introTextLines) / sizeof(introTextLines[0]);

  // Skip intro?
  if (mouseLeft()) {
    intro.linesComplete = lineCount;
    intro.charsComplete = 0;
    goToTitleScreen(game);
    return;
  }

  // Run out of lines?
  if (intro.linesComplete >= lineCount) {
    return;
  }

  // At the end of a line?
  const char *const line = introTextLines[intro.linesComplete];
  if (line[intro.charsComplete] == '\0') {
    ++intro.linesComplete;
    intro.charsComplete = 0;
    return;
  }

  // Show another character
  ++intro.charsComplete;

  // At the end of a line?
  if (line[intro.charsComplete] == '\0') {
    ++intro.linesComplete;
    intro.charsComplete = 0;
    return;
  }
}
