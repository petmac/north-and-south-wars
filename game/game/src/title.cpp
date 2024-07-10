#include "title.h"

#include "game/callbacks.h"
#include "game/title.h"

void startTitle(Title &title) {}

void updateTitle(Title &title, Game &game) {
  // Continue?
  if (mouseLeft()) {
    loadCampaignMap(game);
    return;
  }
}
