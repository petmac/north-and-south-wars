#pragma once

#include <hardware/cia.h>

inline volatile CIA &ciaa = *reinterpret_cast<volatile CIA *>(0xbfe001);
inline volatile CIA &ciab = *reinterpret_cast<volatile CIA *>(0xbfd000);
