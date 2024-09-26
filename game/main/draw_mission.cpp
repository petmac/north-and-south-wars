#include "draw_mission.h"

#include "draw_mission_encounter.h"
#include "draw_mission_tiles.h"

#include "game/mission/mission.h"

void drawMission(Background &background, FrameFast &frameFast,
                 const Mission &mission) {
  switch (mission.state) {
  case MissionState::encounter:
    drawMissionEncounter(background, frameFast, mission);
    break;
  default:
    drawMissionTiles(background, frameFast, mission);
    break;
  }
}
