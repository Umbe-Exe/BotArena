#pragma once

#define infoBox {{0.75,0},{1,1}}

#define maxGeneratorStructure 10000

#define maxEnergy 100

#define maxShield 10000

#define shieldLeakLevel 10000

#define maxMissile 200
#define maxLaser 100
#define minLaser 20

#define botRadius 0.05f
#define weaponRadius 0.01f
#define radarMaxRange 0.15f
#define rangeMaxRange 0.2f
#define sensorEnergyConsumption 2

#define laserDamageMoltiplicator 1.f // already normalized on the 100% laser system level (maxLaser * 1. = max damage)
#define missileBlastRadius 0.1f
#define missileDamage 200
#define laserSpeed 0.5f
#define missileSpeed 0.2f

#define bumpDamage 50
#define bumpForce 0.2f
#define friction 0.2f
#define maxSpeed 0.1f