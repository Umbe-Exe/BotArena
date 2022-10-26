#include "arena_impl.h"
#include "common.h"

void Missile::draw() {

	al_draw_rotated_bitmap(arena.missileBitmap,
			       al_get_bitmap_width(arena.missileBitmap) / 2,
			       al_get_bitmap_height(arena.missileBitmap) / 2,
			       coord.x * arena.arenaSize, coord.y * arena.arenaSize, DEG_PER_RAD * heading, 0);
}

void Missile::update(double delta) {

#ifdef PARTICLES
	if(arena.allowParticles && arena.allowFeedback)
		arena.particles.emplace_back(
			coord.x, coord.y,
			al_map_rgb(rand() % 256, rand() % 256, rand() % 256),
			heading + 150 + 60.f * rand() / RAND_MAX,
			0.1f * rand() / RAND_MAX, 0.2f);
#endif

	for(Bot &bot : arena.bots)
		if(getDistance(coord, bot.coord) < arena.botRadius + arena.weaponRadius) {
			if(bot.shield > arena.shieldLeakLevel) bot.shield -= arena.missileDamage;
			else {
				uint8_t generatorDamage = arena.missileDamage - arena.missileDamage / arena.maxShield * bot.shield;
				bot.shield -= arena.missileDamage + generatorDamage;
				bot.generator -= generatorDamage;
			}

			if(bot.shield < 0) bot.shield = 0;
			if(bot.generator <= 0) arena.addBotToDestroy(&bot);

			for(Bot &botAround : arena.bots)
				if(&botAround != &bot && getDistance(coord, botAround.coord) < arena.missileBlastRadius + arena.botRadius) {

					uint8_t blastDamage = arena.missileDamage - arena.missileDamage * (getDistance(coord, botAround.coord) / (arena.missileBlastRadius + arena.botRadius));

					if(botAround.shield > arena.shieldLeakLevel)
						botAround.shield -= blastDamage;
					else {
						uint8_t generatorDamage = blastDamage - blastDamage / arena.maxShield * botAround.shield;
						botAround.shield -= blastDamage + generatorDamage;
						botAround.generator -= generatorDamage;
					}

					if(botAround.shield < 0) botAround.shield = 0;
					if(botAround.generator <= 0) arena.addBotToDestroy(&botAround);

					botAround.bumping |= BlastWave;
				}
			arena.addWeaponToDestroy(this);

			bot.bumping |= MissileHit;

#ifdef SOUND
			if(arena.allowFeedback && arena.allowSound)
				playSound(SoundType::MISSILEHIT);
#endif
		}

	coord.x += arena.missileSpeed * cosf(heading * DEG_PER_RAD) * delta;
	coord.y += arena.missileSpeed * sinf(heading * DEG_PER_RAD) * delta;

	if(
		coord.x < arena.battleBox.topLeft.x + arena.weaponRadius ||
		coord.x > arena.battleBox.bottomRight.x - arena.weaponRadius ||
		coord.y < arena.battleBox.topLeft.y + arena.weaponRadius ||
		coord.y > arena.battleBox.bottomRight.y - arena.weaponRadius) {
		for(Bot &botAround : arena.bots)
			if(getDistance(coord, botAround.coord) < arena.missileBlastRadius + arena.botRadius) {

				uint8_t blastDamage = arena.missileDamage - arena.missileDamage * (getDistance(coord, botAround.coord) / (arena.missileBlastRadius + arena.botRadius));

				if(botAround.shield > arena.shieldLeakLevel)
					botAround.shield -= blastDamage;
				else {
					uint8_t generatorDamage = blastDamage - blastDamage / arena.maxShield * botAround.shield;
					botAround.shield -= blastDamage + generatorDamage;
					botAround.generator -= generatorDamage;
				}

				if(botAround.shield < 0) botAround.shield = 0;
				if(botAround.generator <= 0) arena.addBotToDestroy(&botAround);

				botAround.bumping |= BlastWave;
			}
		arena.addWeaponToDestroy(this);

#ifdef SOUND
		if(arena.allowFeedback && arena.allowSound)
			playSound(SoundType::MISSILEHIT);
#endif
	}
}

void Laser::draw() {

	al_draw_rotated_bitmap(arena.laserBitmap,
			       al_get_bitmap_width(arena.laserBitmap) / 2,
			       al_get_bitmap_height(arena.laserBitmap) / 2,
			       coord.x * arena.arenaSize, coord.y * arena.arenaSize, DEG_PER_RAD * heading, 0);
}

void Laser::update(double delta) {

	for(Bot &bot : arena.bots)
		if(getDistance(coord, bot.coord) < arena.botRadius + arena.weaponRadius) {
			if(bot.shield > arena.shieldLeakLevel) bot.shield -= damage;
			else {
				uint8_t generatorDamage = damage - damage / arena.maxShield * bot.shield;
				bot.shield -= damage + generatorDamage;
				bot.generator -= generatorDamage;
			}

			if(bot.shield < 0) bot.shield = 0;
			if(bot.generator <= 0) arena.addBotToDestroy(&bot);

			arena.addWeaponToDestroy(this);

			bot.bumping |= LaserHit;

#ifdef SOUND
			if(arena.allowFeedback && arena.allowSound)
				playSound(SoundType::LASERHIT);
#endif
		}

	coord.x += arena.laserSpeed * cosf(heading * DEG_PER_RAD) * delta;
	coord.y += arena.laserSpeed * sinf(heading * DEG_PER_RAD) * delta;

	if(
		coord.x < arena.battleBox.topLeft.x + arena.weaponRadius ||
		coord.x > arena.battleBox.bottomRight.x - arena.weaponRadius ||
		coord.y < arena.battleBox.topLeft.y + arena.weaponRadius ||
		coord.y > arena.battleBox.bottomRight.y - arena.weaponRadius) {
		arena.addWeaponToDestroy(this);

#ifdef SOUND
		if(arena.allowFeedback && arena.allowSound)
			playSound(SoundType::LASERHIT);
#endif
	}
}

#ifdef PARTICLES
void Missile::createParticleBurst() {

	auto &vec = getParticleData(ParticleType::MISSILEBURST);

	arena.particles.reserve(arena.particles.size() + vec.size());

	for(auto &particle : vec) arena.particles.emplace_back(
		coord.x, coord.y, particle.color, particle.heading, particle.speed,
		particle.timeToLive);
}

void Laser::createParticleBurst() {

	auto &vec = getParticleData(ParticleType::LASERBURST);

	arena.particles.reserve(arena.particles.size() + vec.size());

	for(auto &particle : vec) arena.particles.emplace_back(
		coord.x, coord.y, particle.color, particle.heading, particle.speed,
		particle.timeToLive);
}
#endif