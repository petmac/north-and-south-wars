#include "title.h"

#include "dev.h"

#include "game/callbacks.h"
#include "game/sounds.h"
#include "game/title.h"

void startTitle(Title &title) { title.frameIndex = 0; }

void updateTitle(Title &title, Game &game) {
  // Continue?
  if (dev) {
    titleMouseClicked(game);
    return;
  }

  ++title.frameIndex;
}

void titleMouseClicked(Game &game) {
  play(Sound::ok);
  loadCampaignMap(game);
}
