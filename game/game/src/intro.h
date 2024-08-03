#pragma once

struct Game;
struct Intro;

void startIntro(Intro &intro);
void updateIntro(Intro &intro, Game &game);
void introMouseClicked(Game &game);

// Callbacks
void loadTitleScreen(Game &game);
