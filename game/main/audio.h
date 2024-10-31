#pragma once

#include "util/types.h"

bool audioChannelAlloc();
void audioChannelFree();
void audioChannelInterruptHandler(u16 channelIndex);
