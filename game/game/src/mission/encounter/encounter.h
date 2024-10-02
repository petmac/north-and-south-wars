#pragma once

struct Encounter;
struct MapUnit;
struct Mission;

void startEncounter(Encounter &encounter, const MapUnit &attackingUnit,
                    const MapUnit &defendingUnit);
void updateEncounter(Encounter &encounter, const MapUnit &attackingUnit,
                     const MapUnit &defendingUnit, Mission &mission);

// Callbacks
void encounterFinished(Mission &mission);
