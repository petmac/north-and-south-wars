#include "end_sequence.h"

#include "game/callbacks.h"
#include "game/end_sequence.h"
#include "game/sounds.h"

void startEndSequence(EndSequence &) {}

void updateEndSequence(EndSequence &) {}

void endSequenceMouseClicked(Game &game) {
  play(Sound::ok);
  endSequenceFinished(game);
}
