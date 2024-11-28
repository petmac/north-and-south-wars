#pragma once

struct Game;
struct MissionWon;

void startMissionWon(MissionWon &missionWon);
void updateMissionWon(MissionWon &missionWon);
void missionWonMouseClicked(Game &game);

// Callbacks
void missionWonFinished(Game &game);
