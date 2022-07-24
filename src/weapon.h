#pragma once
#include "component.h"
#include "utils.h"
#include "config.h"

struct Weapon : drawable, updatable{
	float heading;
	Coord coord;

	Weapon(float heading, Coord coord) : heading(heading), coord(coord) {}
};

struct Missile : Weapon{

	Missile(float heading, Coord coord) : Weapon(heading, coord) {}

	void draw() override;
	void update(double delta) override;
};

struct Laser : Weapon {
	float damage;

	Laser(float damage, float heading, Coord coord) : damage(damage), Weapon(heading, coord) {}

	void draw() override;
	void update(double delta) override;
};