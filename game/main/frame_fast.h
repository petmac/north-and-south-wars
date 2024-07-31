#pragma once

#include "game/map.h" // maxMapWidth, maxMapHeight

constexpr u16 maxDirtyTiles = maxMapWidth * maxMapHeight;

enum class FrameState : u8 {
  cleared,
  drawnLoading,
  drawnIntro,
  drawnTitle,
  drawnCampaignMap,
  drawnMission,
  drawnError,
};

struct IntroFrameState {
  u16 linesDrawn;
  u16 charsDrawn;
};

struct TileCoords {
  u8 column;
  u8 row;
};

struct DirtyTileList {
  u16 count;
  TileCoords coords[maxDirtyTiles];
};

struct MissionFrameState {
  DirtyTileList dirtyTiles;
};

struct FrameFast {
  FrameState state;
  IntroFrameState intro;
  MissionFrameState mission;
};
