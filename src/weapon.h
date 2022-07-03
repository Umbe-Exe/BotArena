#pragma once
#include "component.h"
#include <allegro5/allegro.h>
#include <allegro5/allegro_primitives.h>

struct Weapon : drawable, updatable{
	float heading, x, y;
	ALLEGRO_BITMAP *bitmap;
	float speed, damage;
};

struct Missile : Weapon{
	void draw() override;
	void update(float delta) override;
};

struct Laser : Weapon {
	void draw() override;
	void update(float delta) override;
};