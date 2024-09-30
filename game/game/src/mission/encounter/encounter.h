#pragma once

struct Encounter;
struct MapUnit;
struct Mission;
struct UnitInstance;

void startEncounter(Encounter &encounter, const UnitInstance &attackingUnit,
                    const MapUnit &attackingMapUnit,
                    const UnitInstance &defendingUnit,
                    const MapUnit &defendingMapUnit);
void updateEncounter(Encounter &encounter, const UnitInstance &attackingUnit,
                     const MapUnit &attackingMapUnit,
                     const UnitInstance &defendingUnit,
                     const MapUnit &defendingMapUnit, Mission &mission);

// Callbacks
void encounterFinished(Mission &mission);
