#pragma once

#include "types.h"

enum class Resource : u8 {
  smallFont,
  palette,
};

bool load(Resource resource);
