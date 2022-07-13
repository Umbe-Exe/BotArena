#pragma once
#include "component.h"
#include "utils.h"
#include <allegro5/allegro.h>
#include <allegro5/allegro_primitives.h>
#include <vector>

struct Sensor : drawable, updatable {
	float angle, range;
	bool enabled;
	int data = 0;
	ALLEGRO_COLOR color;

	virtual void priming() {}
	virtual ~Sensor() {}
};

struct Radar : Sensor{
	float width;
	ALLEGRO_BITMAP *bitmap = nullptr;

	Radar(float angle, float width, float range, ALLEGRO_COLOR color) {
		this->angle = angle;
		this->width = width;
		this->range = range;
		this->color = color;
	}

	void priming() override;

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

	ALLEGRO_BITMAP *bitmap = nullptr;

	float heading, leftTreadSpeed, rightTreadSpeed;
	Coord coord;

	uint8_t energy, shield, missile, laser;
	uint8_t shieldChargeRate, missileChargeRate, laserChargeRate;

	std::vector<Sensor *> sensors;

	bool bumping, alive;

	~Bot() {
		for(auto &sensor : sensors) delete sensor;

		al_destroy_bitmap(bitmap);
	}

	void draw() override;
	void update(double delta) override;
};