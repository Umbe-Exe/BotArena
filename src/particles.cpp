#include "particles.h"
#include "utils.h"
#include "data.h"
#include <vector>

struct Particle {
	float x;
	float y;
	ALLEGRO_COLOR color;
	ALLEGRO_COLOR initialC;
	float heading;
	float speed;
	float timeToLive;
	float initialTTL;
};

std::vector<Particle> particles;

void AddParticle(float x, float y, ALLEGRO_COLOR color, float heading, float speed, float timeToLive){

	particles.push_back({x, y, color, color, heading, speed, timeToLive, timeToLive});
}

void updateParticles(double delta) {

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

void createMissileParticleTrail(Missile *missile) {

	AddParticle(missile->coord.x, missile->coord.y,
				al_map_rgb(rand() % 256, rand() % 256, rand() % 256),
				missile->heading + 150 + 60.f * rand() / RAND_MAX,
				0.1f * rand() / RAND_MAX, 0.2f);
}

void createBotCollisionBurst(float x, float y) {

	for(uint16_t particleCount = 0; particleCount < 100; ++particleCount)
		AddParticle(x, y,
					al_map_rgb(rand() % 256, rand() % 256, rand() % 256),
					360.f * rand() / RAND_MAX,
					0.1f * rand() / RAND_MAX, 1.f);
}

void createBotExplosionBurst(float x, float y) {

	for(uint16_t particleCount = 0; particleCount < 1000; ++particleCount)
		AddParticle(x, y,
					al_map_rgb(rand() % 256, rand() % 256, rand() % 256),
					360.f * rand() / RAND_MAX,
					0.3f * rand() / RAND_MAX, 3.f);
}

void drawPrticles() {

	for(Particle &particle : particles)
		al_draw_pixel(particle.x * arenaSize, particle.y * arenaSize, particle.color);
}
