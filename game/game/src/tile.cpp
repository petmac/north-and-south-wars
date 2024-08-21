#include "game/tile.h"

#include "game/terrain.h"

const Terrain tileTerrain[static_cast<u16>(TileIndex::count)] = {
    // Row 0
    Terrain::plain,
    Terrain::mountain,
    Terrain::mountain,
    Terrain::woods,
    Terrain::road,
    Terrain::road,
    Terrain::road,
    Terrain::road,
    Terrain::road,
    Terrain::road,
    Terrain::bridge,
    Terrain::bridge,
    Terrain::invalid,
    Terrain::invalid,
    Terrain::invalid,
    Terrain::invalid,

    // Row 1
    Terrain::sea,
    Terrain::sea,
    Terrain::sea,
    Terrain::sea,
    Terrain::sea,
    Terrain::sea,
    Terrain::sea,
    Terrain::sea,
    Terrain::sea,
    Terrain::sea,
    Terrain::sea,
    Terrain::sea,
    Terrain::sea,
    Terrain::invalid,
    Terrain::invalid,
    Terrain::invalid,
};
