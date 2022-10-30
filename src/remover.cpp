#include "arena_impl.h"
#include <iostream>
void Arena_Impl::addBotToDestroy(Bot *bot) {

	exploding_bot.insert(bot - bots.data());

	bot->controller->onDeath();

#ifdef SOUND
	playSound(SoundType::BOTEXPLOSION);
#endif
#ifdef PARTICLES
	if(allowFeedback && allowParticles) {
		auto &vec = getParticleData(ParticleType::BOTEXPLOSION);

		particles.reserve(particles.size() + vec.size());

		for(auto &particle : vec) particles.emplace_back(
			bot->coord.x, bot->coord.y, particle.color, particle.heading, particle.speed,
			particle.timeToLive);
	}
#endif
}

void Arena_Impl::addWeaponToDestroy(Weapon *weapon) {
	uint8_t i = 0;
	while(weapons[i] != weapon) ++i;
	exploding_weapon.insert(i);

#ifdef PARTICLES
	if(allowFeedback && allowParticles) weapon->createParticleBurst();
#endif
}

void Arena_Impl::destroyBotsAndWeapons() {

	if(exploding_bot.size()) {
		for(auto it = exploding_bot.rbegin(); it != exploding_bot.rend(); ++it) {
			terminatedBots.push_back(bots[*it].name);
			bots.erase(bots.begin() + *it);
		}

		exploding_bot.clear();
		if(allowFeedback && allowInfobox) primeInfoboxBitmap();
		al_set_target_backbuffer(window);
	}

	if(exploding_weapon.size()) {
		for(auto it = exploding_weapon.rbegin(); it != exploding_weapon.rend(); ++it) 
			weapons.erase(weapons.begin() + *it);
		exploding_weapon.clear();
	}
}
