#include "draw_mission.h"

#include "draw_mission_attack.h"
#include "draw_mission_tiles.h"

#include "game/mission/mission.h"

void drawMission(Background &background, FrameFast &frameFast,
                 const Mission &mission) {
  switch (mission.state) {
  case MissionState::attack:
    drawMissionAttack(background, frameFast, mission);
    break;
  default:
    drawMissionTiles(background, frameFast, mission);
    break;
  }
}
