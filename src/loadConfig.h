#pragma once
#ifndef ARENA_STATIC
    #ifdef _WIN32
        #ifdef EXPORTS
            #define ARENA_EXPORT __declspec(dllexport)
        #else
            #define ARENA_EXPORT __declspec(dllimport)
        #endif
    #else
        #define ARENA_EXPORT
    #endif
#else
    #define ARENA_EXPORT
#endif

void ARENA_EXPORT lockParameters();
void ARENA_EXPORT loadConfigFile(const char *filename = "config.txt");

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
Game ARENA_EXPORT knowGameParameters();