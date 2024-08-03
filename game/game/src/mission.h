#pragma once

struct Game;
struct Mission;

void startMission(Mission &mission);
void updateMission(Mission &mission, Game &game);
void missionMouseClicked(Mission &mission);
