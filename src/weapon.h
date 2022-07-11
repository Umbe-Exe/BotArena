#pragma once
#include "component.h"
#include "utils.h"
#include <allegro5/allegro.h>
#include <allegro5/allegro_primitives.h>

struct Weapon : drawable, updatable{
	float heading;
	Coord coord;
};

struct Missile : Weapon{
	void draw() override;
	void update(double delta) override;
};

struct Laser : Weapon {
	float damage;

	Laser(float damage) {
		this->damage = damage;
	}

	void draw() override;
	void update(double delta) override;
};