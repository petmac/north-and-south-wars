#include "game/unit_defs.h"

#include "game/movement_types.h"
#include "game/unit_types.h"

// https://warswiki.org/wiki/Advance_Wars#Units
static constexpr UnitDef unitDefs[static_cast<u16>(UnitType::count)] = {
    // Infantry
    // https://warswiki.org/wiki/Infantry_(unit)
    {
        .movementPoints = 3,
        .movementType = MovementType::infantry,
    },
    // Mech
    // https://warswiki.org/wiki/Mechanised_Infantry
    {
        .movementPoints = 2,
        .movementType = MovementType::mech,
    },
    // Light tank
    // https://warswiki.org/wiki/Tank
    {
        .movementPoints = 6,
        .movementType = MovementType::tread,
    },
    // Medium tank
    // https://warswiki.org/wiki/Medium_Tank
    {
        .movementPoints = 5,
        .movementType = MovementType::tread,
    },
};

const UnitDef &unitDefForType(UnitType unitType) {
  return unitDefs[static_cast<u16>(unitType)];
}
