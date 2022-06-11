#pragma once
#include "component.h"
#include <allegro5/allegro.h>
#include <allegro5/allegro_primitives.h>

struct Sensor {
	float angle, width, range;
	bool enabled;
	ALLEGRO_COLOR color;
	ALLEGRO_BITMAP *bitmap;

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
	const char *name, *image;
	ALLEGRO_COLOR color;

	ALLEGRO_BITMAP *bitmap;

	float x, y, heading, leftTreadSpeed, rightTreadSpeed;

	uint8_t energy, shield, missile, laser;
	uint8_t shieldChargeRate, missileChargeRate, laserChargeRate;

	Sensor *sensor;
	uint8_t nOfSensors;

	bool bumping, alive;

	void draw() override;
	void update(float delta) override;
};