#pragma once
#include "common.h"
#include "arena_impl.h"

struct Sensor {
	float angle, range;
	bool enabled = 1;
	float data = 0;

	Arena_Impl &arena;

	Sensor(Arena_Impl &arena, float angle, float range) : arena(arena), angle(angle), range(range) {}

	virtual void draw() = 0;
	virtual void update(double delta) = 0;
	virtual void priming() {}
	virtual ~Sensor() {}
	virtual Sensor *clone() = 0;
};

struct Radar : Sensor {
	float width;
	ALLEGRO_BITMAP *bitmap = nullptr;

	Radar(Arena_Impl &arena, float width, float angle, float range) : width(width), Sensor(arena, angle, range) {
		if(arena.allowFeedback) priming();
	}

	void priming() override;

	void draw() override;
	void update(double delta) override;

	~Radar() override {
		if(bitmap) al_destroy_bitmap(bitmap);
	}
	Sensor *clone() {
		return new Radar(arena, width, angle, range);
	}
};

struct LaserRange : Sensor{

	LaserRange(Arena_Impl &arena, float angle, float range) : Sensor(arena, angle, range) {}

	void draw() override;
	void update(double delta) override;
	Sensor *clone() {
		return new LaserRange(arena, angle, range);
	}
};

struct Bot {

	Bot(Arena_Impl *arena, const char *name, const char *image, ALLEGRO_COLOR color, Controller *controller) :
		arena(arena),
		generator(arena->maxGeneratorStructure),
		shield(arena->maxShield),
		missile(arena->maxMissile),
		laser(arena->maxLaser),
		name(name),
		image(image),
		color(color),
		controller(controller) {}

	Bot &operator=(const Bot &bot);

	const char *name, *image;
	ALLEGRO_COLOR color;

	ALLEGRO_BITMAP *bitmap = nullptr;

	float heading = 0;
	float leftTreadSpeed = 0, rightTreadSpeed = 0;
	Coord coord{};

	float generator, shield, missile, laser;
	float shieldChargeRate = 0, missileChargeRate = 0, laserChargeRate = 0;

	std::vector<Sensor *> sensors;

	char bumping = 0;
	float impulseSpeed = 0, impulseHeading = 0;

	Arena_Impl *arena;
	Controller *controller;

	~Bot() {
		for(Sensor *sensor : sensors) delete sensor;

		if(bitmap) al_destroy_bitmap(bitmap);
	}

	void draw();
	void update(double delta);
};