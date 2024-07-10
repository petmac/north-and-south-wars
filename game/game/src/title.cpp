#include "title.h"

#include "dev.h"

#include "game/callbacks.h"
#include "game/title.h"

void startTitle(Title &title) {}

void updateTitle(Title &title, Game &game) {
  // Continue?
  if (dev || mouseLeft()) {
    loadCampaignMap(game);
    return;
  }
}
