#pragma once

#include "util/types.h"

enum class Terrain : u8;

enum class TileIndex : u8 {
  plain,
  smallMountain,
  largeMountain,
  woods,
  roadNorthToEast,
  verticalRoad,
  roadWestToNorth,
  roadEastToSouth,
  horizontalRoad,
  roadSouthToWest,
  horizontalBridge,
  verticalBridge,
  unused1,
  unused2,
  unused3,
  unused4,
  water,
  cliffsNorth,
  cliffsNorthEast,
  cliffsEast,
  cliffsEastSouth,
  cliffsSouth,
  cliffsSouthWest,
  cliffsWest,
  cliffsWestNorth,
  cliffsNorthEastCorner,
  cliffsSouthEastCorner,
  cliffsSouthWestCorner,
  cliffsNorthWestCorner,
  unused5,
  unused6,
  unused7,
  count,
};

Terrain tileTerrain(TileIndex tileIndex);
