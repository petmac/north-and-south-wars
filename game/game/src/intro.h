#pragma once

struct Game;
struct Intro;

void updateIntro(Intro &intro, Game &game);

// Callbacks
void goToTitleScreen(Game &game);
