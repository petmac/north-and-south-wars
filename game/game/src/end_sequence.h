#pragma once

struct Game;
struct EndSequence;

void startEndSequence(EndSequence &endSequence);
void updateEndSequence(EndSequence &endSequence);
void endSequenceMouseClicked(Game &game);

// Callbacks
void endSequenceFinished(Game &game);
