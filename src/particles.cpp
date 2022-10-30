#ifdef PARTICLES
#include "arena_impl.h"

std::vector<Particle> laserBurst;
std::vector<Particle> missileBurst;
std::vector<Particle> botCollision;
std::vector<Particle> botExplosion;

void loadParticles() {

	for(uint16_t particleCount = 0; particleCount < 200; ++particleCount)
		laserBurst.emplace_back(0, 0,
								al_map_rgb(255, 0, 0),
								360.f * rand() / RAND_MAX,
								0.1f * rand() / RAND_MAX, .25f);
	for(uint16_t particleCount = 0; particleCount < 200; ++particleCount)
		missileBurst.emplace_back(0, 0,
								  al_map_rgb(156 + rand() % 100, rand() % 30, rand() % 30),
								  360.f * rand() / RAND_MAX,
								  0.3f * rand() / RAND_MAX, .4f);
	for(uint16_t particleCount = 0; particleCount < 100; ++particleCount)
		botCollision.emplace_back(0, 0,
								  al_map_rgb(rand() % 256, rand() % 256, rand() % 256),
								  360.f * rand() / RAND_MAX,
								  0.1f * rand() / RAND_MAX, 1.f);
	for(uint16_t particleCount = 0; particleCount < 1000; ++particleCount)
		botExplosion.emplace_back(0, 0,
								  al_map_rgb(rand() % 256, rand() % 256, rand() % 256),
								  360.f * rand() / RAND_MAX,
								  0.3f * rand() / RAND_MAX, 3.f);
}

const std::vector<Particle> &getParticleData(ParticleType type) {
	switch(type) {
		case ParticleType::LASERBURST:
			return laserBurst;
		case ParticleType::MISSILEBURST:
			return missileBurst;
		case ParticleType::BOTCOLLISION:
			return botCollision;
		default:
			return botExplosion;
	}
}

Particle::Particle(float x, float y, ALLEGRO_COLOR color, float heading, float speed, float timeToLive) :
	x(x), y(y), color(color),
	initialC(color), heading(heading),
	speed(speed), timeToLive(timeToLive),
	initialTTL(timeToLive) {};

void Arena_Impl::updateParticles(double delta) {

	float dist, radians, fadePct, addPct;
	unsigned char r, g, b;
	
	for(uint32_t i = 0; i < particles.size(); ++i) {

		radians = particles[i].heading * DEG_PER_RAD;
		dist = particles[i].speed * delta;
		particles[i].x += dist * cosf(radians);
		particles[i].y += dist * sinf(radians);
		fadePct = particles[i].timeToLive / particles[i].initialTTL;
		addPct = 1.f - fadePct;

		al_unmap_rgb(particles[i].initialC, &r, &g, &b);

		particles[i].color = al_map_rgb(
			r * fadePct + 200 * addPct,
			g * fadePct + 200 * addPct,
			b * fadePct + 200 * addPct);
		particles[i].timeToLive -= delta;

		if(particles[i].timeToLive < 0) particles.erase(particles.begin() + i--);
	}
}

void Arena_Impl::drawPrticles() {

	for(Particle &particle : particles)
		al_draw_pixel(particle.x * arenaSize, particle.y * arenaSize, particle.color);
}

#endif
