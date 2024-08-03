#pragma once

struct Game;
struct Title;

void startTitle(Title &title);
void updateTitle(Title &title, Game &game);
void titleMouseClicked(Game &game);

// Callbacks
void loadCampaignMap(Game &game);
