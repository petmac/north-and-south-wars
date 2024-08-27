#include "game/mission/movement_cost.h"

#include "game/mission/movement_types.h"
#include "game/mission/terrain.h"

Cost calculateMovementCost(Terrain terrain, MovementType movementType) {
  switch (terrain) {
    // https://warswiki.org/wiki/Bridge
  // https://warswiki.org/wiki/Road
  case Terrain::bridge:
  case Terrain::road:
    switch (movementType) {
    case MovementType::infantry:
    case MovementType::mech:
    case MovementType::tread:
      return 1;
    }
    break;

    // https://warswiki.org/wiki/Mountain
  case Terrain::mountain:
    switch (movementType) {
    case MovementType::infantry:
      return 2;
    case MovementType::mech:
      return 1;
    case MovementType::tread:
      return maxCost;
    }
    break;

    // https://warswiki.org/wiki/Plain
  case Terrain::plain:
    switch (movementType) {
    case MovementType::infantry:
    case MovementType::mech:
    case MovementType::tread:
      return 1;
    }
    break;

    // https://warswiki.org/wiki/Sea
  case Terrain::sea:
  case Terrain::invalid:
    return maxCost;

    // https://warswiki.org/wiki/Woods
  case Terrain::woods:
    switch (movementType) {
    case MovementType::infantry:
    case MovementType::mech:
      return 1;
    case MovementType::tread:
      return 2;
    }
    break;
  }

  return maxCost;
}
