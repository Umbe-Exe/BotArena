#include "weapon.h"
#include "data.h"
#include "remover.h"
#include "sound.h"
#include "competition.h"
#include "particles.h"

void Missile::draw() {

	al_draw_rotated_bitmap(missileBitmap,
			       al_get_bitmap_width(missileBitmap) / 2,
			       al_get_bitmap_height(missileBitmap) / 2,
			       coord.x * arenaSize, coord.y * arenaSize, DEG_PER_RAD * heading, 0);
}

void Missile::update(double delta) {

	if(allowParticles) createMissileParticleTrail(this);

	for(Bot *bot : bots)
		if(getDistance(coord, bot->coord) < botRadius + weaponRadius) {
			if(bot->shield > shieldLeakLevel) bot->shield -= missileDamage;
			else {
				uint8_t generatorDamage = missileDamage - missileDamage / maxShield * bot->shield;
				bot->shield -= missileDamage + generatorDamage;
				bot->generator -= generatorDamage;
			}

			if(bot->shield < 0) bot->shield = 0;
			if(bot->generator <= 0) addBotToDestroy(bot);

			for(Bot *botAround : bots)
				if(botAround != bot && getDistance(coord, botAround->coord) < missileBlastRadius + botRadius) {

					uint8_t blastDamage = missileDamage - missileDamage * (getDistance(coord, botAround->coord) / (missileBlastRadius + botRadius));

					if(botAround->shield > shieldLeakLevel)
						botAround->shield -= blastDamage;
					else {
						uint8_t generatorDamage = blastDamage - blastDamage / maxShield * botAround->shield;
						botAround->shield -= blastDamage + generatorDamage;
						botAround->generator -= generatorDamage;
					}

					if(botAround->shield < 0) botAround->shield = 0;
					if(botAround->generator <= 0) addBotToDestroy(botAround);

					botAround->bumping |= BlastWave;
				}
			addWeaponToDestroy(this);

			bot->bumping |= MissileHit;

			playMissileHitSound();
		}

	coord.x += missileSpeed * cosf(heading * DEG_PER_RAD) * delta;
	coord.y += missileSpeed * sinf(heading * DEG_PER_RAD) * delta;

	if(
		coord.x < battleBox.topLeft.x + weaponRadius ||
		coord.x > battleBox.bottomRight.x - weaponRadius ||
		coord.y < battleBox.topLeft.y + weaponRadius ||
		coord.y > battleBox.bottomRight.y - weaponRadius) {
		for(Bot *botAround : bots)
			if(getDistance(coord, botAround->coord) < missileBlastRadius + botRadius) {

				uint8_t blastDamage = missileDamage - missileDamage * (getDistance(coord, botAround->coord) / (missileBlastRadius + botRadius));

				if(botAround->shield > shieldLeakLevel)
					botAround->shield -= blastDamage;
				else {
					uint8_t generatorDamage = blastDamage - blastDamage / maxShield * botAround->shield;
					botAround->shield -= blastDamage + generatorDamage;
					botAround->generator -= generatorDamage;
				}

				if(botAround->shield < 0) botAround->shield = 0;
				if(botAround->generator <= 0) addBotToDestroy(botAround);

				botAround->bumping |= BlastWave;
			}
		addWeaponToDestroy(this);

		playMissileHitSound();
	}
}

void Missile::createParticleBurst() {

	for(uint16_t particleCount = 0; particleCount < 200; ++particleCount)
		AddParticle(coord.x, coord.y,
					al_map_rgb(156 + rand() % 100, rand() % 30, rand() % 30),
					360.f * rand() / RAND_MAX,
					0.3f * rand() / RAND_MAX, .4f);
}

void Laser::draw() {

	al_draw_rotated_bitmap(laserBitmap,
			       al_get_bitmap_width(laserBitmap) / 2,
			       al_get_bitmap_height(laserBitmap) / 2,
			       coord.x * arenaSize, coord.y * arenaSize, DEG_PER_RAD * heading, 0);
}

void Laser::update(double delta) {

	for(Bot *bot : bots)
		if(getDistance(coord, bot->coord) < botRadius + weaponRadius) {
			if(bot->shield > shieldLeakLevel) bot->shield -= damage;
			else {
				uint8_t generatorDamage = damage - damage / maxShield * bot->shield;
				bot->shield -= damage + generatorDamage;
				bot->generator -= generatorDamage;
			}

			if(bot->shield < 0) bot->shield = 0;
			if(bot->generator <= 0) addBotToDestroy(bot);

			addWeaponToDestroy(this);

			bot->bumping |= LaserHit;

			playLaserHitSound();
		}

	coord.x += laserSpeed * cosf(heading * DEG_PER_RAD) * delta;
	coord.y += laserSpeed * sinf(heading * DEG_PER_RAD) * delta;

	if(
		coord.x < battleBox.topLeft.x + weaponRadius ||
		coord.x > battleBox.bottomRight.x - weaponRadius ||
		coord.y < battleBox.topLeft.y + weaponRadius ||
		coord.y > battleBox.bottomRight.y - weaponRadius) {
		addWeaponToDestroy(this);

		playLaserHitSound();
	}
}

void Laser::createParticleBurst() {

	for(uint16_t particleCount = 0; particleCount < 200; ++particleCount)
		AddParticle(coord.x, coord.y,
					al_map_rgb(255, 0, 0), 
					360.f * rand() / RAND_MAX,
					0.1f * rand() / RAND_MAX, .25);
}
