#pragma once
#include "component.h"
#include <allegro5/allegro.h>
#include <allegro5/allegro_primitives.h>

enum SensorType {
	RANGE, RADAR
};

struct Sensor {
	float angle, width, range;
	bool enabled;
	ALLEGRO_COLOR color;
	ALLEGRO_BITMAP *bitmap;

	int data;
	SensorType type;

	virtual void draw() = 0;
};

struct Radar : Sensor{

	void draw() override;
};

struct LaserRange : Sensor{

	void draw() override;
};

struct Bot : drawable, updatable{

	void (*initFn)();
	void (*updateFn)(float);
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