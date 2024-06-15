#pragma once

#include <hardware/custom.h>

inline volatile Custom *const custom =
    reinterpret_cast<volatile Custom *>(0xdff000UL);
