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

struct TileCoord {
  u8 column;
  u8 row;
};

struct MissionFrameState {
  u16 dirtyTileCount;
  TileCoord dirtyTiles[maxDirtyTiles];
};

struct FrameFast {
  FrameState state;
  IntroFrameState intro;
  MissionFrameState mission;
};
