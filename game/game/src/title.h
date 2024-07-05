#pragma once

struct Game;
struct Title;

void updateTitle(Title &title, Game &game);

// Callbacks
void goToCampaignMap(Game &game);
