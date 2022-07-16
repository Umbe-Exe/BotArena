#include "bot.h"
#include "data.h"
#include "utils.h"
#include "set"

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

	if(enabled) {
		al_draw_rotated_bitmap(bitmap,
							   al_get_bitmap_width(bitmap) / 2,
							   al_get_bitmap_height(bitmap) / 2,
							   currBot->coord.x * arenaSize, currBot->coord.y * arenaSize, RAD_PER_DEG * (currBot->heading + angle), 0);
	}
}

void Radar::update(double delta) {}

void LaserRange::draw() {

	if(enabled) {
		Coord pointCoord = {(float)cos((currBot->heading + angle) * RAD_PER_DEG) * (rangeMaxRange / 100 * (range - data)), (float)sin((currBot->heading + angle) * RAD_PER_DEG) * (rangeMaxRange / 100 * (range - data))};
		pointCoord += currBot->coord;

		al_draw_line(currBot->coord.x * arenaSize, currBot->coord.y * arenaSize, pointCoord.x * arenaSize, pointCoord.y * arenaSize,
					 {currBot->color.r, currBot->color.g, currBot->color.b, 1.f}, 0.003f * arenaSize);

		al_draw_filled_circle(pointCoord.x * arenaSize, pointCoord.y * arenaSize, 0.005f * arenaSize, {currBot->color.r, currBot->color.g, currBot->color.b, 1.f});
	}
}

inline float fakeDistance(Coord a, Coord b) {
	return (a.x - b.x) * (a.x - b.x) + (a.y - b.y) * (a.y - b.y);
}

inline float Signed2DTriArea(Coord a, Coord b, Coord c) {
	return (a.x - c.x) * (b.y - c.y) - (a.y - c.y) * (b.x - c.x);
}

inline void segment_segment_intersection(Coord a, Coord b, Coord c, Coord d, std::set<float> &distance) {
	float a1 = Signed2DTriArea(a, b, d);
	float a2 = Signed2DTriArea(a, b, c);

	if(a1 * a2 < 0.f) {
		float a3 = Signed2DTriArea(c, d, a);
		float a4 = a3 + a2 - a1;

		if(a3 * a4 < 0.f) {
			float t = a3 / (a3 - a4);

			distance.insert(fakeDistance(currBot->coord, a + (b - a) * t));
		}
	}
}

void LaserRange::update(double delta) {

	data = 0;

	std::set<float> distance;

	Coord d = {cosf((currBot->heading + angle) * RAD_PER_DEG) * (rangeMaxRange / 100 * range), sinf((currBot->heading + angle) * RAD_PER_DEG) * (rangeMaxRange / 100 * range)};
	Coord m;
	float b, c, discr, t;

	for(Bot *bot : bots)
		if(bot != currBot) {

			m = currBot->coord - bot->coord;
			b = m.dot(d);
			c = m.dot(m) - botRadius * botRadius;

			if((c > 0.f && b > 0.f) ||
			   (discr = b * b - c) < 0.f) continue;

			t = -b - sqrtf(discr);

			distance.insert(fakeDistance(currBot->coord, currBot->coord + d * t));
		}

	d += currBot->coord;

	segment_segment_intersection(currBot->coord, d, battleBox.topLeft, {battleBox.bottomRight.x,battleBox.topLeft.y}, distance);
	segment_segment_intersection(currBot->coord, d, {battleBox.bottomRight.x,battleBox.topLeft.y}, battleBox.bottomRight, distance);
	segment_segment_intersection(currBot->coord, d, battleBox.bottomRight, {battleBox.topLeft.x,battleBox.bottomRight.y}, distance);
	segment_segment_intersection(currBot->coord, d, {battleBox.topLeft.x,battleBox.bottomRight.y}, battleBox.topLeft, distance);

	if(distance.size()) data = 100 - (*distance.begin()) / rangeMaxRange * 100;
}

void Bot::draw() {

	currBot = this;

	for(Sensor *sensor : sensors) sensor->draw();

	al_draw_rotated_bitmap(bitmap,
			       al_get_bitmap_width(bitmap) / 2,
			       al_get_bitmap_height(bitmap) / 2,
			       coord.x * arenaSize, coord.y * arenaSize, RAD_PER_DEG * heading, 0);
}

void Bot::update(double delta) {

	currBot = this;

	for(Sensor *sensor : sensors) sensor->update(delta);
}
