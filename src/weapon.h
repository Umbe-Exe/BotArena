#pragma once
#include "component.h"
#include <allegro5/allegro.h>
#include <allegro5/allegro_primitives.h>

struct Weapon : drawable, updatable{
	float heading, x, y;
};

struct Missile : Weapon{
	const float speed = 1, damage = 2;

	void draw() override;
	void update(float delta) override;
};

struct Laser : Weapon {
	const float speed = 2, damage = 1;

	void draw() override;
	void update(float delta) override;
};