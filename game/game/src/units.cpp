#include "game/units.h"

// https://warswiki.org/wiki/Advance_Wars#Units
const UnitDef unitDefs[static_cast<u16>(UnitType::count)] = {
    // Infantry
    // https://warswiki.org/wiki/Infantry_(unit)
    {
        .movement = 3,
    },
    // Mech
    // https://warswiki.org/wiki/Mechanised_Infantry
    {
        .movement = 2,
    },
    // Light tank
    // https://warswiki.org/wiki/Tank
    {
        .movement = 6,
    },
    // Medium tank
    // https://warswiki.org/wiki/Medium_Tank
    {
        .movement = 5,
    },
};
