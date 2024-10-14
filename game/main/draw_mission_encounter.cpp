#include "draw_mission_encounter.h"

#include "blit.h" // clear()
#include "chip.h"
#include "frame_fast.h"

#include "game/mission/mission.h" // Mission
#include "game/mission/terrain.h" // Terrain
#include "game/screen.h"          // screenWidth

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

static void drawBackgroundSlices(Background &dst,
                                 const EncounterBackground &src, u16 xWords,
                                 u16 firstSlice, u16 sliceAfterLast) {
  for (u16 sliceIndex = firstSlice,
           y = firstSlice * encounterBackgroundSliceHeight;
       sliceIndex < sliceAfterLast;
       ++sliceIndex, y += encounterBackgroundSliceHeight) {
    blitFast(dst, src[sliceIndex], xWords, y);
  }
}

static void drawBackground(Background &dst, const EncounterBackground &src,
                           u16 xWords) {
  drawBackgroundSlices(dst, src, xWords, 0, encounterBackgroundSliceCount);
}

void drawMissionEncounter(Background &background, FrameFast &frameFast,
                          const Mission &mission) {
  // TODO Refactor into more helper functions
  const MapUnit &attackingUnit = mission.map.units[mission.selectedUnitIndex];
  const MapUnit &defendingUnit = mission.map.units[mission.defendingUnitIndex];
  const MapUnit &leftUnit =
      attackingUnit.force == Force::north ? attackingUnit : defendingUnit;
  const MapUnit &rightUnit =
      attackingUnit.force == Force::north ? defendingUnit : attackingUnit;
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

  // Draw the background if needed.
  switch (frameFast.state) {
  case FrameState::drawnMissionEncounter: {
    // Already drawn similar, so just overwrite the animating parts
    constexpr u16 firstSlice = (encounterBackgroundSliceCount / 2) - 1;
    constexpr u16 sliceAfterLast = firstSlice + 2;
    drawBackgroundSlices(background, leftBackground, leftXWords, firstSlice,
                         sliceAfterLast);
    drawBackgroundSlices(background, rightBackground, rightXWords, firstSlice,
                         sliceAfterLast);
  } break;
  default:
    // Draw the whole background
    drawBackground(background, leftBackground, leftXWords);
    drawBackground(background, rightBackground, rightXWords);
    frameFast.state = FrameState::drawnMissionEncounter;
    break;
  }

  // Draw the units
  blitFast(background, chip.mission.encounter.units.forces[0].units[0], 5, 128);
  blitFast(background, chip.mission.encounter.units.forces[1].units[0], 15,
           128);
}
