#include "loadConfig.h"
#include "config.h"
#include <allegro5/allegro.h>
#include <string>

int maxGeneratorStructure = 10000;

int maxEnergy = 100;

int maxShield = 10000;

int shieldLeakLevel = 10000;

int maxMissile = 200;
int maxLaser = 100;
int minLaser = 20;

float botRadius = 0.05f;
float weaponRadius = 0.01f;
float radarMaxRange = 0.15f;
float rangeMaxRange = 0.2f;
int sensorEnergyConsumption = 2;

float laserDamageMoltiplicator = 1.f;
float missileBlastRadius = 0.1f;
int missileDamage = 200;
float laserSpeed = 0.5f;
float missileSpeed = 0.2f;

int bumpDamage = 50;
float bumpForce = 0.2f;
float friction = 0.2f;
float maxSpeed = 0.1f;

bool allowSound = true;
bool allowParticles = true;

bool alreadyConfigured = false;

void lockParameters() {
	alreadyConfigured = true;
}

void loadConfigFile(const char *filename) {

	if(!alreadyConfigured) {
		ALLEGRO_CONFIG *config = al_load_config_file(filename);

		if(!config) {
			config = al_create_config();

			al_set_config_value(config, "BOT LEVELS", "generator_structure", "10000");
			al_set_config_value(config, "BOT LEVELS", "generator_power", "100");
			al_set_config_value(config, "BOT LEVELS", "shield", "10000");
			al_set_config_value(config, "BOT LEVELS", "shield_leak_level", "10000");
			al_set_config_value(config, "BOT LEVELS", "missile", "200");
			al_set_config_value(config, "BOT LEVELS", "laser", "100");
			al_set_config_value(config, "BOT LEVELS", "min_laser_activation", "20");
			al_set_config_value(config, "BOT LEVELS", "sensor_power_consumption", "2");

			al_set_config_value(config, "PROPORTIONS", "bot_radius", "0.05f");
			al_set_config_value(config, "PROPORTIONS", "weapon_radius", "0.01f");
			al_set_config_value(config, "PROPORTIONS", "bot_speed", "0.1f");
			al_set_config_value(config, "PROPORTIONS", "missile_speed", "0.2f");
			al_set_config_value(config, "PROPORTIONS", "laser_speed", "0.5f");
			al_set_config_value(config, "PROPORTIONS", "missile_blast_radius", "0.1f");
			al_set_config_value(config, "PROPORTIONS", "bump_force", "0.2f");
			al_set_config_value(config, "PROPORTIONS", "friction", "0.2f");
			al_set_config_value(config, "PROPORTIONS", "radar_max_range", "0.15f");
			al_set_config_value(config, "PROPORTIONS", "laser_range_max_range", "0.2f");

			al_set_config_value(config, "WEAPON DAMAGE", "missile_damage", "200");
			al_set_config_value(config, "WEAPON DAMAGE", "laser_damage_moltiplicator", "1.f");
			al_set_config_value(config, "WEAPON DAMAGE", "bump_damage", "50");

			al_set_config_value(config, "EFFECTS", "allow_sound", "1");
			al_set_config_value(config, "EFFECTS", "allow_particles", "1");

			al_save_config_file(filename, config);
			al_destroy_config(config);
		} else {
			std::string newFile = "The file (" + std::string(filename) + ") has a problem.txt";

			const char *value;

			if(value = al_get_config_value(config, "BOT LEVELS", "generator_structure"))
				maxGeneratorStructure = atoi(value);
			else goto repeat;
			if(value = al_get_config_value(config, "BOT LEVELS", "generator_power"))
				maxEnergy = atoi(value);
			else goto repeat;
			if(value = al_get_config_value(config, "BOT LEVELS", "shield"))
				maxShield = atoi(value);
			else goto repeat;
			if(value = al_get_config_value(config, "BOT LEVELS", "shield_leak_level"))
				shieldLeakLevel = atoi(value);
			else goto repeat;
			if(value = al_get_config_value(config, "BOT LEVELS", "missile"))
				maxMissile = atoi(value);
			else goto repeat;
			if(value = al_get_config_value(config, "BOT LEVELS", "laser"))
				maxLaser = atoi(value);
			else goto repeat;
			if(value = al_get_config_value(config, "BOT LEVELS", "sensor_power_consumption"))
				sensorEnergyConsumption = atoi(value);
			else goto repeat;
			if(value = al_get_config_value(config, "PROPORTIONS", "bot_radius"))
				botRadius = atof(value);
			else goto repeat;
			if(value = al_get_config_value(config, "PROPORTIONS", "weapon_radius"))
				weaponRadius = atof(value);
			else goto repeat;
			if(value = al_get_config_value(config, "PROPORTIONS", "bot_speed"))
				maxSpeed = atof(value);
			else goto repeat;
			if(value = al_get_config_value(config, "PROPORTIONS", "missile_speed"))
				missileSpeed = atof(value);
			else goto repeat;
			if(value = al_get_config_value(config, "PROPORTIONS", "laser_speed"))
				laserSpeed = atof(value);
			else goto repeat;
			if(value = al_get_config_value(config, "PROPORTIONS", "missile_blast_radius"))
				missileBlastRadius = atof(value);
			else goto repeat;
			if(value = al_get_config_value(config, "PROPORTIONS", "bump_force"))
				bumpForce = atof(value);
			else goto repeat; 
			if(value = al_get_config_value(config, "PROPORTIONS", "friction"))
				friction = atof(value);
			else goto repeat;
			if(value = al_get_config_value(config, "PROPORTIONS", "radar_max_range"))
				radarMaxRange = atof(value);
			else goto repeat;
			if(value = al_get_config_value(config, "PROPORTIONS", "laser_range_max_range"))
				rangeMaxRange = atof(value);
			else goto repeat;
			if(value = al_get_config_value(config, "WEAPON DAMAGE", "missile_damage"))
				missileDamage = atoi(value);
			else goto repeat;
			if(value = al_get_config_value(config, "WEAPON DAMAGE", "laser_damage_moltiplicator"))
				laserDamageMoltiplicator = atof(value);
			else goto repeat;
			if(value = al_get_config_value(config, "WEAPON DAMAGE", "bump_damage"))
				bumpDamage = atoi(value);
			else goto repeat;
			if(value = al_get_config_value(config, "EFFECTS", "allow_sound"))
				allowSound = atoi(value);
			else goto repeat;
			if(value = al_get_config_value(config, "EFFECTS", "allow_particles"))
				allowParticles = atoi(value);
			else goto repeat;

			alreadyConfigured = true;

			repeat:
			if(!alreadyConfigured) loadConfigFile(newFile.c_str());

			al_destroy_config(config);
		}
	}
}

Game knowGameParameters() {
	return
	{
		maxGeneratorStructure,

		maxEnergy,

		maxShield,

		shieldLeakLevel,

		maxMissile,
		maxLaser,
		minLaser,

		botRadius,
		weaponRadius,
		radarMaxRange,
		rangeMaxRange,
		sensorEnergyConsumption,
	
		laserDamageMoltiplicator,
		missileBlastRadius,
		missileDamage,
		laserSpeed,
		missileSpeed,

		bumpDamage,
		bumpForce,
		friction,
		maxSpeed,

		allowSound,
		allowParticles
	};
}