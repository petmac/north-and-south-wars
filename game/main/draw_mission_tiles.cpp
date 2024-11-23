#include "draw_mission_tiles.h"

#include "blit.h"
#include "chip.h"
#include "frame_fast.h"
#include "text.h"

#include "game/mission/mission.h"

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
  drawText(background, chip.main.smallFont, 0, 0, text);

  // Add tiles to the dirty tile list
  constexpr u16 tileCount = (charCount + 1) / 2;
  for (u16 column = 0; column < tileCount; ++column) {
    addDirtyTile(dirtyTiles, column, 0);
  }
}

static void drawTile(Background &background, u16 column, u16 row,
                     const Map &map) {
  const TileIndex tileIndex = map.tiles[row][column];
  const TileBitmap &tile =
      chip.mission.tileset.tiles[static_cast<u16>(tileIndex)];
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
  if (frameFast.state == FrameState::drawnMissionTiles) {
    restoreDirtyTiles(background, frameFast.mission.dirtyTiles, map);
  } else {
    // Not sure what's on the background, so draw all tiles
    for (u16 row = 0; row < map.height; ++row) {
      for (u16 column = 0; column < map.width; ++column) {
        drawTile(background, column, row, map);
      }
    }

    // Start using the dirty tile list
    frameFast.state = FrameState::drawnMissionTiles;
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
                     const MapUnit &unit, u16 frameIndex) {
  const ForceBitmaps &forceBitmaps =
      chip.mission.units.forces[static_cast<u16>(unit.force)];
  const UnitBitmap &src = forceBitmaps.units[static_cast<u16>(unit.type)];

  const u16 y = unit.coords.row * tileHeight;
  blitPartFast(background, src, unit.coords.column, y, frameIndex, 0, 1,
               unitHeight);
  addDirtyTile(dirtyTiles, unit.coords.column, unit.coords.row);

  // Draw unit health
  if (unit.health < 10) {
    const char hp[2] = {
        static_cast<char>('0' + unit.health),
        '\0',
    };
    drawText(background, chip.main.smallFont, unit.coords.column * 2 + 1, y + 8,
             hp);
  }
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
      chip.mission.arrows.bitmaps[static_cast<u16>(ArrowType::reachable)];
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
      chip.mission.arrows.bitmaps[static_cast<u16>(arrowTipType)];
  drawBobOnTile(background, dirtyTiles, goal.column, goal.row, arrowTipBitmap);

  // Draw the trunk of the arrow
  TileCoords next = goal;
  TileCoords prev = pathfinding.cameFrom[current.row][current.column];
  while (prev != current) {
    const ArrowType arrowTrunkType = computeArrowTrunk(prev, current, next);
    const ArrowBitmap &arrowTrunkBitmap =
        chip.mission.arrows.bitmaps[static_cast<u16>(arrowTrunkType)];
    drawBobOnTile(background, dirtyTiles, current.column, current.row,
                  arrowTrunkBitmap);

    // Step towards the start of the arrow
    next = current;
    current = prev;
    prev = pathfinding.cameFrom[current.row][current.column];
  }
}

static void drawAttackableUnits(Background &background,
                                DirtyTileList &dirtyTiles,
                                const Attackable &attackable,
                                const MapUnit (&units)[maxMapUnits]) {
  const ArrowBitmap &attackableBitmap =
      chip.mission.arrows.bitmaps[static_cast<u16>(ArrowType::attackable)];

  for (u16 attackableUnitIndex = 0; attackableUnitIndex < attackable.unitCount;
       ++attackableUnitIndex) {
    const u16 unitIndex = attackable.unitIndices[attackableUnitIndex];
    const MapUnit &unit = units[unitIndex];
    drawBobOnTile(background, dirtyTiles, unit.coords.column, unit.coords.row,
                  attackableBitmap);
  }
}

template <u16 buttonCount>
static void drawMenu(Background &background, DirtyTileList &dirtyTiles,
                     const MenuButtonBitmapIndex (&buttons)[buttonCount]) {
  // Draw buttons
  constexpr u16 menuXWords = menuX / 16;

  for (u16 buttonIndex = 0; buttonIndex < buttonCount; ++buttonIndex) {
    const MenuButtonBitmap &src =
        chip.mission.menu.buttons[static_cast<u16>(buttons[buttonIndex])];
    const u16 y = menuY + (buttonIndex * menuButtonHeight);
    blitFast(background, src, menuXWords, y);
  }

  // Add dirty tiles
  constexpr u16 menuX1Tiles = menuX / tileWidth;
  constexpr u16 menuY1Tiles = menuY / tileHeight;
  constexpr u16 menuX2Tiles = ((menuX + menuButtonWidth - 1) / tileWidth) + 1;
  constexpr u16 menuY2Tiles =
      ((menuY + (buttonCount * menuButtonHeight) - 1) / tileHeight) + 1;

  for (u16 tileRow = menuY1Tiles; tileRow < menuY2Tiles; ++tileRow) {
    for (u16 tileColumn = menuX1Tiles; tileColumn < menuX2Tiles; ++tileColumn) {
      addDirtyTile(dirtyTiles, tileColumn, tileRow);
    }
  }
}

void drawMissionTiles(Background &background, FrameFast &frameFast,
                      const Mission &mission) {
  const Map &map = mission.map;
  makeBackgroundPristine(background, frameFast, map);

  // Draw state-specific stuff under units
  DirtyTileList &dirtyTiles = frameFast.mission.dirtyTiles;
  switch (mission.state) {
  case MissionState::playerSelectAttackOrWait:
  case MissionState::playerSelectTarget:
    drawAttackableUnits(background, dirtyTiles, mission.attackable, map.units);
    break;
  default:
    break;
  }

  // TODO What a terrible hack.
  static u16 frameForAnimation = 0;
  ++frameForAnimation;

  // Draw units
  for (u16 unitIndex = 0; unitIndex < map.unitCount; ++unitIndex) {
    const MapUnit &unit = map.units[unitIndex];

    // Don't draw dead units
    if (unit.health == 0) {
      continue;
    }

    // Animate units which haven't been moved
    const u16 frameForAnimationAdjusted = frameForAnimation + (unitIndex * 2);
    const u16 frameIndex =
        (!unit.moved && (frameForAnimationAdjusted & 16)) ? 1 : 0;

    // Draw unit
    drawUnit(background, dirtyTiles, unit, frameIndex);
  }

  // Draw state-specific stuff on top of units
  switch (mission.state) {
  case MissionState::intro:
    drawMissionText(background, dirtyTiles, "Intro");
    break;
  case MissionState::startOfTurn:
    drawMissionText(background, dirtyTiles, "Start of turn");
    break;
  case MissionState::resupply:
    break;
  case MissionState::playerSelectUnit:
    break;
  case MissionState::playerConfirmEndTurn: {
    const MenuButtonBitmapIndex buttons[] = {
        MenuButtonBitmapIndex::endTurn,
    };
    drawMenu(background, dirtyTiles, buttons);
  } break;
  case MissionState::playerSelectUnitDestination:
    drawPath(background, dirtyTiles, mission.pathfinding,
             mission.unitDestination);
    break;
  case MissionState::movingPlayerUnit:
    break;
  case MissionState::playerSelectAttackOrWait: {
    const MenuButtonBitmapIndex buttons[] = {
        MenuButtonBitmapIndex::attack,
        MenuButtonBitmapIndex::wait,
    };
    drawMenu(background, dirtyTiles, buttons);
  } break;
  case MissionState::playerSelectWait: {
    const MenuButtonBitmapIndex buttons[] = {
        MenuButtonBitmapIndex::wait,
    };
    drawMenu(background, dirtyTiles, buttons);
  } break;
  case MissionState::playerSelectTarget:
    drawMissionText(background, dirtyTiles, "Select target");
    break;
  case MissionState::playerEncounter:
    break;
  case MissionState::movingEnemyUnit:
  case MissionState::enemyEncounter:
    drawMissionText(background, dirtyTiles, "Enemy turn");
    break;
  }
}
