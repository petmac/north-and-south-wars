#pragma once

struct Encounter;
struct Mission;

void startEncounter(Encounter &encounter);
void updateEncounter(Encounter &encounter, Mission &mission);

// Callbacks
void encounterFinished(Mission &mission);
