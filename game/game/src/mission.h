#pragma once

#include "util/types.h"

struct Game;
struct Mission;

void startMission(Mission &mission);
void updateMission(Mission &mission, Game &game);
void missionMouseClicked(Mission &mission, u16 mouseX, u16 mouseY);
