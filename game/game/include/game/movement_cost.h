#pragma once

#include "util/types.h"

enum class MovementType : u8;
enum class Terrain : u8;

using Cost = u16;

constexpr Cost maxCost = 65535;

Cost calculateMovementCost(Terrain terrain, MovementType movementType);
