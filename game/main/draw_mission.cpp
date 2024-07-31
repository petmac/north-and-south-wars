#include "draw_mission.h"

#include "blit.h"
#include "chip.h"
#include "frame_fast.h"
#include "text.h"

#include "game/mission.h"

static void addDirtyTile(DirtyTileList &dirtyTiles, u16 column, u16 row) {
  // Going to overflow the dirty tile list?
  if (dirtyTiles.count >= maxDirtyTiles) {
    return;
  }

  const TileCoords coords = {
      .column = static_cast<u8>(column),
      .row = static_cast<u8>(row),
  };

  dirtyTiles.coords[dirtyTiles.count++] = coords;
}

template <u16 charCount>
static void drawMissionText(Background &background, DirtyTileList &dirtyTiles,
                            const char (&text)[charCount]) {
  drawText(background, chip.smallFont, 0, 0, text);

  // Add tiles to the dirty tile list
  constexpr u16 tileCount = (charCount + 1) / 2;
  for (u16 column = 0; column < tileCount; ++column) {
    addDirtyTile(dirtyTiles, column, 0);
  }
}

static void drawTile(Background &background, u16 column, u16 row,
                     const Mission &mission) {
  const u16 tileIndex = mission.map.tiles[row][column];
  const TileBitmap &tile = chip.tileset.tiles[tileIndex];
  blitFast(background, tile, column, row * tileHeight);
}

static void restoreDirtyTiles(Background &background, DirtyTileList &dirtyTiles,
                              const Mission &mission) {
  for (u16 dirtyTileIndex = 0; dirtyTileIndex < dirtyTiles.count;
       ++dirtyTileIndex) {
    const TileCoords &coords = dirtyTiles.coords[dirtyTileIndex];
    drawTile(background, coords.column, coords.row, mission);
  }

  dirtyTiles.count = 0;
}

static void makeBackgroundPristine(Background &background, FrameFast &frameFast,
                                   const Mission &mission) {
  // If a mission has already been drawn, then respect dirty tile list
  if (frameFast.state == FrameState::drawnMission) {
    restoreDirtyTiles(background, frameFast.mission.dirtyTiles, mission);
  } else {
    // Not sure what's on the background, so draw all tiles
    for (u16 row = 0; row < mission.map.height; ++row) {
      for (u16 column = 0; column < mission.map.width; ++column) {
        drawTile(background, column, row, mission);
      }
    }

    // Start using the dirty tile list
    frameFast.state = FrameState::drawnMission;
    frameFast.mission.dirtyTiles.count = 0;
  }
}

static void drawUnit(Background &background, DirtyTileList &dirtyTiles,
                     u16 column, u16 row, Force force, UnitType unit) {
  const ForceBitmaps &forceBitmaps = chip.units.forces[static_cast<u16>(force)];
  const UnitBitmap &src = forceBitmaps.units[static_cast<u16>(unit)];

  blitFast(background, src, column, row * tileHeight);
  addDirtyTile(dirtyTiles, column, row);
}

void drawMission(Background &background, FrameFast &frameFast,
                 const Mission &mission) {
  makeBackgroundPristine(background, frameFast, mission);

  // Draw state-specific stuff
  DirtyTileList &dirtyTiles = frameFast.mission.dirtyTiles;
  switch (mission.state) {
  case MissionState::intro:
    drawMissionText(background, dirtyTiles, "Intro");
    break;
  case MissionState::startOfTurn:
    drawMissionText(background, dirtyTiles, "Start of turn");
    break;
  case MissionState::resupply:
    drawMissionText(background, dirtyTiles, "Resupply");
    break;
  case MissionState::selectUnit:
    drawMissionText(background, dirtyTiles, "Select unit");
    break;
  case MissionState::selectUnitDestination:
    drawMissionText(background, dirtyTiles, "Select unit destination");
    break;
  case MissionState::movingUnit:
    drawMissionText(background, dirtyTiles, "Moving unit");
    break;
  case MissionState::selectUnitAction:
    drawMissionText(background, dirtyTiles, "Select unit action");
    break;
  case MissionState::selectTarget:
    drawMissionText(background, dirtyTiles, "Select target");
    break;
  }

  // Draw units
  drawUnit(background, dirtyTiles, 1, 2, Force::north, UnitType::infantry);
}
