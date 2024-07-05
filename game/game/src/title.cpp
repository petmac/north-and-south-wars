#include "title.h"

#include "game/callbacks.h"
#include "game/title.h"

void updateTitle(Title &title, Game &game) {
  // Continue?
  if (mouseLeft()) {
    goToCampaignMap(game);
    return;
  }
}
