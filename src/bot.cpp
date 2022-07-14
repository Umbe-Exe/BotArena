#include "bot.h"
#include "data.h"
#include "utils.h"

void Radar::priming() {

	if(bitmap) al_destroy_bitmap(bitmap);

	float radius = radarMaxRange / 100 * range * arenaSize;

	bitmap = al_create_bitmap(radius * 2, radius * 2);

	ALLEGRO_DISPLAY *target = al_get_current_display();
	al_set_target_bitmap(bitmap);

	al_draw_arc(radius, radius, radius / 2.f, RAD_PER_DEG * (angle - width / 2), RAD_PER_DEG * (width), {currBot->color.r, currBot->color.g, currBot->color.b, 0.5f}, radius);

	al_set_target_backbuffer(target);
}

void Radar::draw() {

	al_draw_rotated_bitmap(bitmap,
			       al_get_bitmap_width(bitmap) / 2,
			       al_get_bitmap_height(bitmap) / 2,
						   currBot->coord.x * arenaSize, currBot->coord.y * arenaSize, RAD_PER_DEG * (currBot->heading + angle), 0);
}

void Radar::update(double delta) {}

void LaserRange::draw() {

	Coord pointCoord = {(float)cos((currBot->heading + angle) * RAD_PER_DEG) * (rangeMaxRange / 100 * (range - data)), (float)sin((currBot->heading + angle) * RAD_PER_DEG) * (rangeMaxRange / 100 * (range - data))};
	pointCoord += currBot->coord;

	al_draw_line(currBot->coord.x * arenaSize, currBot->coord.y * arenaSize, pointCoord.x * arenaSize, pointCoord.y * arenaSize,
				 {currBot->color.r, currBot->color.g, currBot->color.b, 1.f}, 0.003f * arenaSize);

	al_draw_filled_circle(pointCoord.x * arenaSize, pointCoord.y * arenaSize, 0.005f * arenaSize, {currBot->color.r, currBot->color.g, currBot->color.b, 1.f});
}

void LaserRange::update(double delta) {}

void Bot::draw() {

	currBot = this;

	for(Sensor *sensor : sensors) sensor->draw();

	al_draw_rotated_bitmap(bitmap,
			       al_get_bitmap_width(bitmap) / 2,
			       al_get_bitmap_height(bitmap) / 2,
			       coord.x * arenaSize, coord.y * arenaSize, RAD_PER_DEG * heading, 0);
}

void Bot::update(double delta) {}
