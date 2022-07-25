#pragma once
#include "component.h"
#include "utils.h"
#include "config.h"
#include <allegro5/allegro.h>
#include <vector>

struct Sensor : drawable, updatable {
	float angle, range;
	bool enabled = 1;
	float data = 0;

	Sensor(float angle, int range) : angle(angle), range(range) {}

	virtual void priming() {}
	virtual ~Sensor() {}
};

struct Radar : Sensor {
	float width;
	ALLEGRO_BITMAP *bitmap = nullptr;

	Radar(float width, float angle, int range) : width(width), Sensor(angle, range) {
		priming();
	}

	void priming() override;

	void draw() override;
	void update(double delta) override;

	~Radar() override {
		al_destroy_bitmap(bitmap);
	}
};

struct LaserRange : Sensor{

	LaserRange(float angle, int range) : Sensor(angle, range) {}

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

	float heading = 0;
	int leftTreadSpeed = 0, rightTreadSpeed = 0;
	Coord coord;

	float generator = maxGeneratorStructure, shield = maxShield, missile = maxMissile, laser = maxLaser;
	int shieldChargeRate = 50, missileChargeRate = 50, laserChargeRate = 0;

	std::vector<Sensor *> sensors;

	bool bumping = 0;
	float impulseSpeed = 0, impulseHeading = 0;

	~Bot() {
		for(Sensor *sensor : sensors) delete sensor;

		al_destroy_bitmap(bitmap);
	}

	void draw() override;
	void update(double delta) override;
};