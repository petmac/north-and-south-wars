#include "intro.h"

#include "dev.h"

#include "game/callbacks.h"
#include "game/intro.h"

const char *const introTextLines[] = {
    // 0000000111111111122222222223333333333", //
    // 3456789012345678901234567890123456789", //
    "              - Notice -",                //
    "",                                        //
    "This game is an unofficial sequel to",    //
    "North & South, a parody game based on",   //
    "the American Civil War.",                 //
    "",                                        //
    "This game does not glamourise war, nor",  //
    "does it seek to trivialise the",          //
    "suffering war causes.",                   //
    "",                                        //
    "The game contains imagery such as the",   //
    "Confederate flag, but it does not",       //
    "condone or support the racist beliefs",   //
    "that the flag is often associated with.", //
    "",                                        //
    "There are real wars ongoing in the",      //
    "world right now, with thousands of",      //
    "innocent civilians being killed. We",     //
    "should have courage to discuss war",      //
    "openly, to criticise governments,",       //
    "politicians and corporations, in words",  //
    "and other art forms.",                    //
    "",                                        //
    "An 'Advance Wars' style game is",         //
    "appropriate commentary, as Nintendo",     //
    "delayed the release of 'Advance Wars",    //
    "1+2: Re-Boot Camp' following Russia's",   //
    "invasion of Ukraine.",                    //
    "",                                        //
    "- Click left mouse button to continue -", //
    // 0000000111111111122222222223333333333", //
    // 3456789012345678901234567890123456789", //
};

void startIntro(Intro &intro) {
  intro.linesComplete = 0;
  intro.charsComplete = 0;
}

void updateIntro(Intro &intro, Game &game) {
  constexpr u16 lineCount = sizeof(introTextLines) / sizeof(introTextLines[0]);

  // Dev skip.
  if (dev && (intro.linesComplete >= 3)) {
    loadTitleScreen(game);
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

void introMouseClicked(Game &game) { loadTitleScreen(game); }
