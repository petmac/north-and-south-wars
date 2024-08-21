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
  const TileIndex tileIndex = map.tiles[row][column];
  const TileBitmap &tile = chip.tileset.tiles[static_cast<u16>(tileIndex)];
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

static constexpr ArrowType computeArrowTip(const TileCoords &current,
                                           const TileCoords &goal) {
  if (current.column < goal.column) {
    return ArrowType::pointEast;
  } else if (current.column > goal.column) {
    return ArrowType::pointWest;
  } else if (current.row < goal.row) {
    return ArrowType::pointSouth;
  } else {
    return ArrowType::pointNorth;
  }
}

static constexpr ArrowType computeArrowTrunk(const TileCoords &prev,
                                             const TileCoords &current,
                                             const TileCoords &next) {
  if (prev.column < current.column) {
    // From west to...
    if (next.row < current.row) {
      return ArrowType::westToNorth;
    } else if (next.row > current.row) {
      return ArrowType::southToWest;
    } else {
      return ArrowType::horizontal;
    }
  } else if (prev.column > current.column) {
    // From east to...
    if (next.row < current.row) {
      return ArrowType::northToEast;
    } else if (next.row > current.row) {
      return ArrowType::eastToSouth;
    } else {
      return ArrowType::horizontal;
    }
  } else if (prev.row < current.row) {
    // From north to...
    if (next.column < current.column) {
      return ArrowType::westToNorth;
    } else if (next.column > current.column) {
      return ArrowType::northToEast;
    } else {
      return ArrowType::vertical;
    }
  } else {
    // From south to...
    if (next.column < current.column) {
      return ArrowType::southToWest;
    } else if (next.column > current.column) {
      return ArrowType::eastToSouth;
    } else {
      return ArrowType::vertical;
    }
  }
}

static void drawPath(Background &background, DirtyTileList &dirtyTiles,
                     const Pathfinding &pathfinding, const TileCoords &goal) {
  // Draw reachable tiles
  const ArrowBitmap &reachableBitmap =
      chip.arrows.bitmaps[static_cast<u16>(ArrowType::reachable)];
  for (u16 reachableTileIndex = 0;
       reachableTileIndex < pathfinding.reachable.count; ++reachableTileIndex) {
    const TileCoords &coords =
        pathfinding.reachable.locations[reachableTileIndex];
    drawBobOnTile(background, dirtyTiles, coords.column, coords.row,
                  reachableBitmap);
  }

  // Draw the tip of the arrow
  TileCoords current = pathfinding.cameFrom[goal.row][goal.column];
  if (current == goal) {
    return;
  }
  const ArrowType arrowTipType = computeArrowTip(current, goal);
  const ArrowBitmap &arrowTipBitmap =
      chip.arrows.bitmaps[static_cast<u16>(arrowTipType)];
  drawBobOnTile(background, dirtyTiles, goal.column, goal.row, arrowTipBitmap);

  // Draw the trunk of the arrow
  TileCoords next = goal;
  TileCoords prev = pathfinding.cameFrom[current.row][current.column];
  while (prev != current) {
    const ArrowType arrowTrunkType = computeArrowTrunk(prev, current, next);
    const ArrowBitmap &arrowTrunkBitmap =
        chip.arrows.bitmaps[static_cast<u16>(arrowTrunkType)];
    drawBobOnTile(background, dirtyTiles, current.column, current.row,
                  arrowTrunkBitmap);

    // Step towards the start of the arrow
    next = current;
    current = prev;
    prev = pathfinding.cameFrom[current.row][current.column];
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
  case MissionState::selectUnitDestination:
    drawPath(background, dirtyTiles, mission.pathfinding,
             mission.unitDestination);
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
}
