#include "draw_mission.h"

#include "blit.h"
#include "chip.h"
#include "frame_fast.h"
#include "text.h"

#include "game/mission.h"

static void addDirtyTile(DirtyTileList &dirtyTiles, u16 column, u16 row) {
  // Dirty tile already in list?
  for (u16 existingIndex = 0; existingIndex < dirtyTiles.count;
       ++existingIndex) {
    const TileCoords &existing = dirtyTiles.coords[existingIndex];
    if ((existing.column == column) && (existing.row == row)) {
      return;
    }
  }

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
                     const Map &map) {
  const u16 tileIndex = map.tiles[row][column];
  const TileBitmap &tile = chip.tileset.tiles[tileIndex];
  blitFast(background, tile, column, row * tileHeight);
}

static void restoreDirtyTiles(Background &background, DirtyTileList &dirtyTiles,
                              const Map &map) {
  for (u16 dirtyTileIndex = 0; dirtyTileIndex < dirtyTiles.count;
       ++dirtyTileIndex) {
    const TileCoords &coords = dirtyTiles.coords[dirtyTileIndex];
    drawTile(background, coords.column, coords.row, map);
  }

  dirtyTiles.count = 0;
}

static void makeBackgroundPristine(Background &background, FrameFast &frameFast,
                                   const Map &map) {
  // If a mission has already been drawn, then respect dirty tile list
  if (frameFast.state == FrameState::drawnMission) {
    restoreDirtyTiles(background, frameFast.mission.dirtyTiles, map);
  } else {
    // Not sure what's on the background, so draw all tiles
    for (u16 row = 0; row < map.height; ++row) {
      for (u16 column = 0; column < map.width; ++column) {
        drawTile(background, column, row, map);
      }
    }

    // Start using the dirty tile list
    frameFast.state = FrameState::drawnMission;
    frameFast.mission.dirtyTiles.count = 0;
  }
}

static void drawBobOnTile(
    Background &background, DirtyTileList &dirtyTiles, u16 column, u16 row,
    const MaskedInterleavedBitmap<tileWidth, tileHeight, playfieldDepth> &src) {
  blitFast(background, src, column, row * tileHeight);
  addDirtyTile(dirtyTiles, column, row);
}

static void drawUnit(Background &background, DirtyTileList &dirtyTiles,
                     u16 column, u16 row, Force force, UnitType unit) {
  const ForceBitmaps &forceBitmaps = chip.units.forces[static_cast<u16>(force)];
  const UnitBitmap &src = forceBitmaps.units[static_cast<u16>(unit)];

  drawBobOnTile(background, dirtyTiles, column, row, src);
}

static void drawPath(Background &background, DirtyTileList &dirtyTiles,
                     const Pathfinding &pathfinding, TileCoords start,
                     TileCoords goal) {
  // Draw reachable tiles
  for (u16 reachableTileIndex = 0;
       reachableTileIndex < pathfinding.reachable.count; ++reachableTileIndex) {
    const TileCoords reachableTile = nullptr;
  }

  // Draw path to goal
  for (TileCoords coords = goal; coords != start;
       coords = pathfinding.cameFrom[coords.row][coords.column]) {
    const ArrowBitmap &src =
        chip.arrows.bitmaps[static_cast<u16>(ArrowType::pointEast)];
    drawBobOnTile(background, dirtyTiles, coords.column, coords.row, src);
  }
}

void drawMission(Background &background, FrameFast &frameFast,
                 const Mission &mission) {
  const Map &map = mission.map;
  makeBackgroundPristine(background, frameFast, map);

  // Draw units
  DirtyTileList &dirtyTiles = frameFast.mission.dirtyTiles;
  for (u16 unitIndex = 0; unitIndex < map.unitCount; ++unitIndex) {
    const MapUnit &unit = map.units[unitIndex];
    drawUnit(background, dirtyTiles, unit.coords.column, unit.coords.row,
             unit.force, unit.type);
  }

  // Draw state-specific stuff
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
  case MissionState::selectUnitDestination: {
    const MapUnit &selectedUnit = map.units[mission.selectedUnitIndex];
    const TileCoords start = selectedUnit.coords;
    const TileCoords goal = mission.unitDestination;
    drawPath(background, dirtyTiles, mission.pathfinding, start, goal);
    drawMissionText(background, dirtyTiles, "Select unit destination");
  } break;
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
}
