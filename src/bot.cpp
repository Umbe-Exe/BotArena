#include "bot.h"
#include "data.h"
#include "utils.h"

void Radar::priming(int sideLength) {

	if(bitmap) al_destroy_bitmap(bitmap);

	int w = sideLength * (radarMaxRange * range / 100) * 2;
	int half = width / 2;

	bitmap = al_create_bitmap(w, w);
	al_set_target_bitmap(bitmap);

	al_draw_arc(half, half, half, RAD_PER_DEG * (angle - width / 2), RAD_PER_DEG * this->width, {color.r, color.g, color.b, 0.5f}, w);
}

void Radar::draw() {}

void Radar::update(float delta) {}

void LaserRange::draw() {}

void LaserRange::update(float delta) {}

void Bot::draw() {}

void Bot::update(float delta) {}
