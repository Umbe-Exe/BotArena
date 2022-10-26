#pragma once
#ifdef PARTICLES
#include <vector>
#include <allegro5/allegro.h>

struct Particles {
	Particles();
};

enum class ParticleType {
	LASERBURST, MISSILEBURST,
	BOTCOLLISION, BOTEXPLOSION
};

struct Particle {
	float x;
	float y;
	ALLEGRO_COLOR color;
	ALLEGRO_COLOR initialC;
	float heading;
	float speed;
	float timeToLive;
	float initialTTL;

	Particle(float x,
			 float y,
			 ALLEGRO_COLOR color,
			 float heading,
			 float speed,
			 float timeToLive);
};

const std::vector<Particle> getParticleData(ParticleType type);

#endif