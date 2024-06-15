#pragma once

#include <hardware/custom.h>

inline volatile Custom &custom =
    *reinterpret_cast<volatile Custom *const>(0xdff000UL);
