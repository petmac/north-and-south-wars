#pragma once

struct Encounter;
struct MapUnit;
struct Mission;

void startEncounter(Encounter &encounter, const MapUnit &attackingUnit);
void updateEncounter(Encounter &encounter, MapUnit &attackingUnit,
                     MapUnit &defendingUnit, Mission &mission);

// Callbacks
void encounterFinished(Mission &mission);
