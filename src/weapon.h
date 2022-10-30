#pragma once
#include "common.h"
#include "arena_impl.h"

struct Weapon {
	float heading;
	Coord coord;

	Arena_Impl &arena;

	Weapon(Arena_Impl &arena, float heading, Coord coord) : arena(arena), heading(heading), coord(coord) {}

	virtual void draw() = 0;
	virtual void update(double delta) = 0;
#ifdef PARTICLES
	virtual void createParticleBurst() = 0;
#endif
};

struct Missile : Weapon{

	Missile(Arena_Impl &arena, float heading, Coord coord) : Weapon(arena, heading, coord) {}

	void draw() override;
	void update(double delta) override;
#ifdef PARTICLES
	void createParticleBurst() override;
#endif
};

struct Laser : Weapon {
	float damage;

	Laser(Arena_Impl &arena, float damage, float heading, Coord coord) : damage(damage), Weapon(arena, heading, coord) {}

	void draw() override;
	void update(double delta) override;
#ifdef PARTICLES
	void createParticleBurst() override;
#endif
};