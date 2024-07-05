#pragma once

#include "frame_chip.h" // Background

struct CampaignMap;
struct FrameFast;

void drawCampaignMap(Background &background, FrameFast &frameFast,
                     const CampaignMap &campaignMap);
