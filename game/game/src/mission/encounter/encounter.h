#pragma once

struct Encounter;
struct Game;
struct MapUnit;
struct Mission;

void startEncounter(Encounter &encounter, const MapUnit &attackingUnit);
void updateEncounter(Encounter &encounter, MapUnit &attackingUnit,
                     MapUnit &defendingUnit, Mission &mission, Game &game);

// Callbacks
void encounterFinished(Mission &mission, Game &game);
