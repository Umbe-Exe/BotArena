#pragma once
#include "component.h"
#include <allegro5/allegro.h>
#include <allegro5/allegro_primitives.h>

struct Weapon : drawable, updatable{
	float heading, x, y;
};

struct Missile : Weapon{
	void draw() override;
	void update(float delta) override;
};

struct Laser : Weapon {
	float damage;

	Laser(float damage) {
		this->damage = damage;
	}

	void draw() override;
	void update(float delta) override;
};