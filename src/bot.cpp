#include "bot.h"
#include "data.h"
#include "utils.h"

void Radar::priming() {

	if(bitmap) al_destroy_bitmap(bitmap);

	int radius = radarMaxRange / 100 * range * getSmallestSide();

	bitmap = al_create_bitmap(radius * 2, radius * 2);

	ALLEGRO_DISPLAY *target = al_get_current_display();
	al_set_target_bitmap(bitmap);

	al_draw_arc(radius, radius, radius/2, RAD_PER_DEG * (angle - width / 2 - 90), RAD_PER_DEG * (width), {color.r, color.g, color.b, 0.5f}, radius);

	al_set_target_backbuffer(target);
}

void Radar::draw() {

	al_draw_rotated_bitmap(bitmap,
			       al_get_bitmap_width(bitmap) / 2,
			       al_get_bitmap_height(bitmap) / 2,
						   currBot->coord.x * getSmallestSide(), currBot->coord.y * getSmallestSide(), RAD_PER_DEG * (currBot->heading + angle), 0);
}

void Radar::update(double delta) {}

void LaserRange::draw() {

	Coord pointCoord = {(float)cos((currBot->heading + angle - 90) * RAD_PER_DEG) * (rangeMaxRange / 100 * (range - data)), (float)sin((currBot->heading + angle - 90) * RAD_PER_DEG) * (rangeMaxRange / 100 * (range - data))};
	pointCoord += currBot->coord;

	al_draw_line(currBot->coord.x * getSmallestSide(), currBot->coord.y * getSmallestSide(), pointCoord.x * getSmallestSide(), pointCoord.y * getSmallestSide(),
				 {color.r, color.g, color.b, 1.f}, 0.003f * getSmallestSide());

	al_draw_filled_circle(pointCoord.x * getSmallestSide(), pointCoord.y * getSmallestSide(), 0.005f * getSmallestSide(), {color.r, color.g, color.b, 1.f});
}

void LaserRange::update(double delta) {}

void Bot::draw() {

	currBot = this;

	for(auto &sensor : sensors) sensor->draw();

	al_draw_rotated_bitmap(bitmap,
			       al_get_bitmap_width(bitmap) / 2,
			       al_get_bitmap_height(bitmap) / 2,
			       coord.x * getSmallestSide(), coord.y * getSmallestSide(), RAD_PER_DEG * heading, 0);
}

void Bot::update(double delta) {}
