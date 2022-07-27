#pragma once

void lockParameters();
void loadConfigFile(const char *filename = "config.txt");

struct Game{
	int maxGeneratorStructure;

	int maxEnergy;

	int maxShield;

	int shieldLeakLevel;

	int maxMissile;
	int maxLaser;
	int minLaser;

	float botRadius;
	float weaponRadius;
	float radarMaxRange;
	float rangeMaxRange;
	int sensorEnergyConsumption;

	float laserDamageMoltiplicator;
	float missileBlastRadius;
	int missileDamage;
	float laserSpeed;
	float missileSpeed;

	int bumpDamage;
	float bumpForce;
	float friction;
	float maxSpeed;

	bool allowSound;
	bool allowParticles;
};

//The size of the arena is (1.f, 1.f)
Game knowGameParameters();