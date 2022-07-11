#include "bot.h"
#include "data.h"
#include "utils.h"

void Radar::priming(int sideLength) {

	if(bitmap) al_destroy_bitmap(bitmap);

	int w = sideLength * (radarMaxRange / 100 * range) * 2;
	int half = w / 2;

	bitmap = al_create_bitmap(w * 2, w * 2);

	ALLEGRO_DISPLAY *target = al_get_current_display();
	al_set_target_bitmap(bitmap);

	al_draw_arc(w, w, half, RAD_PER_DEG * (angle - width / 2), RAD_PER_DEG * width, {color.r, color.g, color.b, 0.5f}, w);

	al_set_target_backbuffer(target);
}

void Radar::draw() {

	al_draw_rotated_bitmap(bitmap,
			       al_get_bitmap_width(bitmap) / 2,
			       al_get_bitmap_height(bitmap) / 2,
			       currBot->coord.x * getSmallestSide(), currBot->coord.y * getSmallestSide(), currBot->heading + angle, 0);
}

void Radar::update(double delta) {}

void LaserRange::draw() {

	Coord pointCoord = {(float)cos((currBot->heading + angle) * RAD_PER_DEG) * rangeMaxRange / 100 * (range - data), (float)sin((currBot->heading + angle) * RAD_PER_DEG) * rangeMaxRange / 100 * (range - data)};

	al_draw_line(currBot->coord.x * getSmallestSide(), currBot->coord.y * getSmallestSide(), pointCoord.x * getSmallestSide(), pointCoord.y * getSmallestSide(),
				 {color.r, color.g, color.b, 0.5f}, 0.005f * getSmallestSide());

	al_draw_filled_circle(pointCoord.x * getSmallestSide(), pointCoord.y * getSmallestSide(), 0.008f * getSmallestSide(), {color.r, color.g, color.b, 0.5f});
}

void LaserRange::update(double delta) {}

void Bot::draw() {

	currBot = this;

	for(uint8_t i = 0; i < nOfSensors; ++i) sensor[i]->draw();

	al_draw_rotated_bitmap(bitmap,
			       al_get_bitmap_width(bitmap) / 2,
			       al_get_bitmap_height(bitmap) / 2,
			       coord.x * getSmallestSide(), coord.y * getSmallestSide(), heading, 0);
}

void Bot::update(double delta) {}
