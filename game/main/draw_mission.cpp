#include "draw_mission.h"

#include "blit.h"
#include "chip.h"
#include "frame_fast.h"
#include "text.h"

#include "game/mission.h"

template <u16 charCount>
static void drawMissionText(Background &background, FrameFast &frameFast,
                            const char (&text)[charCount]) {
  constexpr u16 tileCount = (charCount + 1) / 2;

  // Going to overflow the dirty tile list?
  if ((frameFast.mission.dirtyTileCount + tileCount) > maxDirtyTiles) {
    return;
  }

  drawText(background, chip.smallFont, 0, 0, text);

  // Add tiles to the dirty tile list
  for (u8 column = 0; column < tileCount; ++column) {
    const TileCoord tileCoord = {
        .column = column,
        .row = 0,
    };

    frameFast.mission.dirtyTiles[frameFast.mission.dirtyTileCount++] =
        tileCoord;
  }
}

static void drawTile(Background &background, u16 column, u16 row,
                     const Mission &mission) {
  const u16 tileIndex = mission.map.tiles[row][column];
  const TileBitmap &tile = chip.tileset.tiles[tileIndex];
  blitFast(background, tile, column, row * tileHeight);
}

static void restoreDirtyTiles(Background &background, FrameFast &frameFast,
                              const Mission &mission) {
  for (u16 dirtyTileIndex = 0;
       dirtyTileIndex < frameFast.mission.dirtyTileCount; ++dirtyTileIndex) {
    const TileCoord &tileCoord = frameFast.mission.dirtyTiles[dirtyTileIndex];
    drawTile(background, tileCoord.column, tileCoord.row, mission);
  }

  frameFast.mission.dirtyTileCount = 0;
}

static void makeBackgroundPristine(Background &background, FrameFast &frameFast,
                                   const Mission &mission) {
  // If a mission has already been drawn, then respect dirty tile list
  if (frameFast.state == FrameState::drawnMission) {
    restoreDirtyTiles(background, frameFast, mission);
  } else {
    // Not sure what's on the background, so draw all tiles
    for (u16 row = 0; row < mission.map.height; ++row) {
      for (u16 column = 0; column < mission.map.width; ++column) {
        drawTile(background, column, row, mission);
      }
    }

    // Start using the dirty tile list
    frameFast.state = FrameState::drawnMission;
    frameFast.mission.dirtyTileCount = 0;
  }
}

void drawMission(Background &background, FrameFast &frameFast,
                 const Mission &mission) {
  makeBackgroundPristine(background, frameFast, mission);

  // Draw state-specific stuff
  switch (mission.state) {
  case MissionState::intro:
    drawMissionText(background, frameFast, "Intro");
    break;
  case MissionState::startOfTurn:
    drawMissionText(background, frameFast, "Start of turn");
    break;
  case MissionState::resupply:
    drawMissionText(background, frameFast, "Resupply");
    break;
  case MissionState::selectUnit:
    drawMissionText(background, frameFast, "Select unit");
    break;
  case MissionState::selectUnitDestination:
    drawMissionText(background, frameFast, "Select unit destination");
    break;
  case MissionState::movingUnit:
    drawMissionText(background, frameFast, "Moving unit");
    break;
  case MissionState::selectUnitAction:
    drawMissionText(background, frameFast, "Select unit action");
    break;
  case MissionState::selectTarget:
    drawMissionText(background, frameFast, "Select target");
    break;
  }
}
