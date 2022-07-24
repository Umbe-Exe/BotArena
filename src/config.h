#pragma once

#define infoBox {{0.75,0},{1,1}}

#define maxEnergy 100
#define maxShield 100
#define maxMissile 100
#define maxLaser 100
#define minLaser 20

#define botRadius 0.05f
#define weaponRadius 0.015f
#define radarMaxRange 0.15f
#define rangeMaxRange 0.2f
#define sensorEnergyConsumption 2

#define laserDamageMoltiplicator 0.05f // already normalized on the 100% laser system level (100 * 0.05 = 5 max damage)
#define missileBlastRadius 0.1f
#define missileDamage 10
#define laserSpeed 0.3f
#define missileSpeed 0.2f

#define shieldLeakLevel 80

#define bumpDamage 15
#define bumpForce 0.2f
#define friction 0.2f
#define maxSpeed 0.1f