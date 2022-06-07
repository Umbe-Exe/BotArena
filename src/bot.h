#pragma once
#include "component.h"
#include <allegro5/allegro.h>
#include <allegro5/allegro_primitives.h>

struct Radar {
	float angle, width, range;
	bool enabled;
};

struct LaserRange {
	float angle, range;
	bool enabled;
};

struct Bot : updatable, drawable {

	void *initFn;
	void *updateFn;

	const char *name;
	ALLEGRO_COLOR color;

	float x, y, heading;

	uint16_t energy, shield, missile, laser;
	uint8_t shieldChargeRate, missileChargeRate, laserChargeRate;

	Radar radar1, radar2;
	LaserRange laserRange1, laserRange2;

	bool bumping;

	void update(float delta) override;
	void draw() override;

};