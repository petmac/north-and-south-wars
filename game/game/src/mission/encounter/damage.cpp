#include "damage.h"

#include "game/mission/terrain.h"
#include "game/mission/unit_types.h"

// https://advancewars.fandom.com/wiki/Damage
static constexpr u16 baseDamageChart[static_cast<u16>(UnitType::count)]
                                    [static_cast<u16>(UnitType::count)] = {
                                        {55, 65, 75, 105},
                                        {45, 55, 70, 95},
                                        {5, 55, 55, 85},
                                        {1, 15, 15, 55},
};

static constexpr u16 terrainDefence(Terrain terrain) {
  // https://awbw.fandom.com/wiki/Terrain
  switch (terrain) {
  case Terrain::invalid:
  case Terrain::road:
  case Terrain::sea:
    return 0;
  case Terrain::bridge:
  case Terrain::plain:
    return 1;
  case Terrain::woods:
    return 2;
  case Terrain::mountain:
    return 4;
  }

  return 0;
};

u16 computeDamage(UnitType attackerType, u16 attackerHealth,
                  UnitType defenderType, u16 defenderHealth,
                  Terrain defenderTerrain) {
  // Based on:
  // https://awbw.fandom.com/wiki/Damage_Formula
  // https://advancewars.fandom.com/wiki/Damage_Formula#Advance_Wars
  const u16 baseDamageOutOf100 =
      baseDamageChart[static_cast<u16>(defenderType)]
                     [static_cast<u16>(attackerType)];
  const u16 damage = (baseDamageOutOf100 * attackerHealth) / 100;
  const u16 defence = (terrainDefence(defenderTerrain) * defenderHealth) / 10;

  if (defence > damage) {
    return 0;
  }

  const u16 damageMinusDefence = damage - defence;
  if (damageMinusDefence > defenderHealth) {
    return defenderHealth;
  }

  return damageMinusDefence;
}
