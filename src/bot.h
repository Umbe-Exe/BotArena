#pragma once
#include "component.h"
#include "utils.h"
#include <allegro5/allegro.h>
#include <allegro5/allegro_primitives.h>

struct Sensor : drawable, updatable {
	float angle, range;
	bool enabled;
	int data;
	ALLEGRO_COLOR color;

	virtual void priming(int sideLength) {}
	virtual ~Sensor() {}
};

struct Radar : Sensor{
	float width;
	ALLEGRO_BITMAP *bitmap;

	Radar(float angle, float width, float range, ALLEGRO_COLOR color) {
		this->angle = angle;
		this->width = width;
		this->range = range;
		this->color = color;
	}

	void priming(int sideLength) override;

	void draw() override;
	void update(double delta) override;

	~Radar() override {
		al_destroy_bitmap(bitmap);
	}
};

struct LaserRange : Sensor{

	LaserRange(float angle, float range, ALLEGRO_COLOR color) {
		this->angle = angle;
		this->range = range;
		this->color = color;
	}

	void draw() override;
	void update(double delta) override;
};

struct Bot : drawable, updatable{

	void (*initFn)();
	void (*updateFn)(double);
	const char *name, *image;
	ALLEGRO_COLOR color;

	ALLEGRO_BITMAP *bitmap;

	float heading, leftTreadSpeed, rightTreadSpeed;
	Coord coord;

	uint8_t energy, shield, missile, laser;
	uint8_t shieldChargeRate, missileChargeRate, laserChargeRate;

	Sensor **sensor;
	uint8_t nOfSensors;

	bool bumping, alive;

	~Bot() {
		for(uint8_t j = 0; j < this->nOfSensors; ++j) delete this->sensor[j];
		free(this->sensor);

		al_destroy_bitmap(bitmap);
	}

	void draw() override;
	void update(double delta) override;
};