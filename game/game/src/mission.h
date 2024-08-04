#pragma once

#include "util/types.h"

struct Mission;

void startMission(Mission &mission);
void updateMission(Mission &mission, u16 mouseX, u16 mouseY);
void missionMouseClicked(Mission &mission, u16 mouseX, u16 mouseY);
