#pragma once
#include "component.h"
#include <allegro5/allegro.h>
#include <allegro5/allegro_primitives.h>

struct Sensor {
	float angle, width, range;
	ALLEGRO_COLOR color;

	virtual void draw() = 0;
};

struct Radar : Sensor{
	bool data;
	void draw() override;
};

struct LaserRange : Sensor{
	float data;
	void draw() override;
};

struct Bot : drawable, updatable{

	void *initFn;
	void *updateFn;

	const char *name;
	ALLEGRO_COLOR color;

	float x, y, heading, leftTreadSpeed, rightTreadSpeed;

	uint16_t energy, shield, missile, laser;
	uint8_t shieldChargeRate, missileChargeRate, laserChargeRate;

	Sensor *sensor[4];

	bool bumping;

	void draw() override;
	void update(float delta) override;
};