#pragma once
#include "component.h"
#include "utils.h"
#include <allegro5/allegro.h>
#include <allegro5/allegro_primitives.h>
#include <vector>

struct Sensor : drawable, updatable {
	float angle, range;
	bool enabled = 1;
	float data = 0;

	Sensor(float angle, float range) : angle(angle), range(range) {}

	virtual void priming() {}
	virtual ~Sensor() {}
};

struct Radar : Sensor {
	int width;
	ALLEGRO_BITMAP *bitmap = nullptr;

	Radar(int width, int angle, int range) : width(width), Sensor(angle, range) {}

	void priming() override;

	void draw() override;
	void update(double delta) override;

	~Radar() override {
		al_destroy_bitmap(bitmap);
	}
};

struct LaserRange : Sensor{

	LaserRange(int angle, int range) : Sensor(angle, range) {}

	void draw() override;
	void update(double delta) override;
};

struct Bot : drawable, updatable{

	Bot(const char *name, const char *image, ALLEGRO_COLOR color, void (*initFn)(), void (*updateFn)(double)) :
		name(name),
		image(image),
		color(color),
		initFn(initFn), updateFn(updateFn) {}

	void (*initFn)();
	void (*updateFn)(double);
	const char *name, *image;
	ALLEGRO_COLOR color;

	ALLEGRO_BITMAP *bitmap = nullptr;

	int heading = 0, leftTreadSpeed = 0, rightTreadSpeed = 0;
	Coord coord;

	uint8_t energy = 100, shield = 100, missile = 100, laser = 100;
	uint8_t shieldChargeRate = 0, missileChargeRate = 0, laserChargeRate = 0;

	std::vector<Sensor *> sensors;

	bool bumping = 0;
	float impulseSpeed = 0, impulseHeading = 0;
	int turboTime = 0;

	~Bot() {
		for(Sensor *sensor : sensors) delete sensor;

		al_destroy_bitmap(bitmap);
	}

	void draw() override;
	void update(double delta) override;
};