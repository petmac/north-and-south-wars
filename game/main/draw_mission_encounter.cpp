#include "draw_mission_encounter.h"

#include "blit.h" // clear()
#include "chip.h"
#include "frame_fast.h"

#include "game/mission/mission.h" // Mission
#include "game/mission/terrain.h" // Terrain
#include "game/screen.h"          // screenWidthInWords

static const EncounterBackground &terrainBackground(Terrain terrain) {
  const EncounterBackgrounds &backgrounds = chip.mission.encounter.backgrounds;
  const EncounterBackground &defaultBackground = backgrounds.plain;

  switch (terrain) {
  case Terrain::bridge:
    return backgrounds.bridge;
  case Terrain::mountain:
    return backgrounds.mountain;
  case Terrain::plain:
    return backgrounds.plain;
  case Terrain::road:
    return backgrounds.road;
  case Terrain::sea:
    return defaultBackground; // Should never happen
  case Terrain::woods:
    return backgrounds.woods;
  case Terrain::invalid:
    return defaultBackground; // Should never happen
  }

  // Should never happen
  return defaultBackground;
}

static void drawBackground(Background &dst, const EncounterBackground &src,
                           u16 xWords) {
  blitFast(dst, src, xWords, 0);
}

static void drawBackgroundPart(Background &dst, const EncounterBackground &src,
                               u16 dstXWords, u16 partY, u16 partHeight) {
  constexpr u16 partWidthWords = (encounterBackgroundWidth + 15) / 16;
  blitPartFast(dst, src, dstXWords, partY, 0, partY, partWidthWords,
               partHeight);
}

static void drawPeople(Background &background,
                       const EncounterPerson people[maxEncounterPeoplePerSide],
                       const MapUnit &unit) {
  const u16 peopleCount = peopleCountForHealth(unit.health);
  for (u16 personIndex = 0; personIndex < peopleCount; ++personIndex) {
    const EncounterPerson &person = people[personIndex];
    blitFast(background,
             chip.mission.encounter.units.forces[static_cast<u16>(unit.force)]
                 .units[static_cast<u16>(unit.type)],
             person.xWords, person.y);
  }
}

void drawMissionEncounter(Background &background, FrameFast &frameFast,
                          const Mission &mission) {
  // TODO Refactor into more helper functions
  const MapUnit &attackingUnit = mission.map.units[mission.selectedUnitIndex];
  const MapUnit &defendingUnit = mission.map.units[mission.defendingUnitIndex];
  const bool attackerIsOnLeft = attackingUnit.force == Force::north;
  const MapUnit &leftUnit = attackerIsOnLeft ? attackingUnit : defendingUnit;
  const MapUnit &rightUnit = attackerIsOnLeft ? defendingUnit : attackingUnit;
  const TileIndex leftTileIndex =
      mission.map.tiles[leftUnit.coords.row][leftUnit.coords.column];
  const TileIndex rightTileIndex =
      mission.map.tiles[rightUnit.coords.row][rightUnit.coords.column];
  const Terrain leftTerrain = tileTerrain(leftTileIndex);
  const Terrain rightTerrain = tileTerrain(rightTileIndex);
  const EncounterBackground &leftBackground = terrainBackground(leftTerrain);
  const EncounterBackground &rightBackground = terrainBackground(rightTerrain);
  constexpr u16 leftXWords = 0;
  constexpr u16 rightXWords = screenWidthInWords / 2;
  const Encounter &encounter = mission.encounter;
  const EncounterPerson(&leftPeople)[maxEncounterPeoplePerSide] =
      attackerIsOnLeft ? encounter.attackingPeople : encounter.defendingPeople;
  const EncounterPerson(&rightPeople)[maxEncounterPeoplePerSide] =
      attackerIsOnLeft ? encounter.defendingPeople : encounter.attackingPeople;

  // Draw the background if needed.
  switch (frameFast.state) {
  case FrameState::drawnMissionEncounter: {
    // Already drawn similar, so just overwrite the animating parts
    constexpr u16 partY = 100;
    constexpr u16 partHeight = 100;
    drawBackgroundPart(background, leftBackground, leftXWords, partY,
                       partHeight);
    drawBackgroundPart(background, rightBackground, rightXWords, partY,
                       partHeight);
  } break;
  default:
    // Draw the whole background
    drawBackground(background, leftBackground, leftXWords);
    drawBackground(background, rightBackground, rightXWords);
    frameFast.state = FrameState::drawnMissionEncounter;
    break;
  }

  // Draw the units
  drawPeople(background, leftPeople, leftUnit);
  drawPeople(background, rightPeople, rightUnit);
}
