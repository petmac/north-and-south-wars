#pragma once

#include "background.h"

struct CampaignMap;
struct FrameFast;

void drawCampaignMap(Background &background, FrameFast &frameFast,
                     const CampaignMap &campaignMap, u16 nextMission);
