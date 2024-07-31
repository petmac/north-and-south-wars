#pragma once

#include "game/map.h" // TileCoords, maxMapWidth, maxMapHeight

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
