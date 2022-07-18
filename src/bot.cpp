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

bool testRayBot(Coord p, Coord d, Coord s) {
	Coord m = p - s;
	float c = m.dot(m) - botRadius * botRadius;

	if(c <= 0.f) return 1;
	float b = m.dot(d);

	if(b > 0.f) return 0;
	float disc = b * b - c;

	if(disc < 0.f) return 0;

	return 1;
}

bool isClockwiseAngle(Coord a, Coord b, Coord c) {
	if((b.x - a.x) * (c.y - a.y) - (c.x - a.x) * (b.y - a.y) < 0) return 1;
	return 0;
}

void Radar::update(double delta) {

	data = -1;

	Coord
		start = {cosf((currBot->heading + angle - width / 2) * RAD_PER_DEG), sinf((currBot->heading + angle - width / 2) * RAD_PER_DEG)},
		end = {cosf((currBot->heading + angle + width / 2) * RAD_PER_DEG), sinf((currBot->heading + angle + width / 2) * RAD_PER_DEG)};

	for(Bot *bot : bots)
		if(bot != currBot) {

			if(getDistance(bot->coord, currBot->coord) < botRadius + radarMaxRange / 100 * range && (
				testRayBot(currBot->coord, start, bot->coord) ||
				testRayBot(currBot->coord, end, bot->coord) || (
					isClockwiseAngle(start, currBot->coord, bot->coord) &&
					isClockwiseAngle(bot->coord, currBot->coord, end)))) {
				data = +1;
				break;
			}
		}
}

void LaserRange::draw() {

	if(enabled) {
		Coord pointCoord = {cosf((currBot->heading + angle) * RAD_PER_DEG) * (rangeMaxRange / 100 * (range - data)), sinf((currBot->heading + angle) * RAD_PER_DEG) * (rangeMaxRange / 100 * (range - data))};
		pointCoord += currBot->coord;

		al_draw_line(currBot->coord.x * arenaSize, currBot->coord.y * arenaSize, pointCoord.x * arenaSize, pointCoord.y * arenaSize,
					 {currBot->color.r, currBot->color.g, currBot->color.b, 1.f}, 0.003f * arenaSize);

		al_draw_filled_circle(pointCoord.x * arenaSize, pointCoord.y * arenaSize, 0.005f * arenaSize, {currBot->color.r, currBot->color.g, currBot->color.b, 1.f});
	}
}

inline float Signed2DTriArea(Coord a, Coord b, Coord c) {
	return (a.x - c.x) * (b.y - c.y) - (a.y - c.y) * (b.x - c.x);
}

inline void segment_segment_intersection(Coord a, Coord b, Coord c, Coord d, std::set<float> &t) {
	float a1 = Signed2DTriArea(a, b, d);
	float a2 = Signed2DTriArea(a, b, c);

	if(a1 * a2 < 0.f) {
		float a3 = Signed2DTriArea(c, d, a);
		float a4 = a3 + a2 - a1;

		if(a3 * a4 < 0.f) t.insert(a3 / (a3 - a4));
	}
}

void LaserRange::update(double delta) {

	data = 0;

	std::set<float> t;

	Coord d = {cosf((currBot->heading + angle) * RAD_PER_DEG), sinf((currBot->heading + angle) * RAD_PER_DEG)};
	Coord m;
	float b, c, discr, tmp;

	for(Bot *bot : bots)
		if(bot != currBot) {

			m = currBot->coord - bot->coord;
			b = m.dot(d);
			c = m.dot(m) - botRadius * botRadius;

			if((c > 0.f && b > 0.f) ||
			   (discr = b * b - c) < 0.f) continue;

			tmp = (-b - sqrtf(discr)) / rangeMaxRange;

			if(tmp < 1.f) t.insert(tmp);
		}

	d *= rangeMaxRange / 100 * range;
	d += currBot->coord;

	segment_segment_intersection(currBot->coord, d, battleBox.topLeft, {battleBox.bottomRight.x,battleBox.topLeft.y}, t);
	segment_segment_intersection(currBot->coord, d, {battleBox.bottomRight.x,battleBox.topLeft.y}, battleBox.bottomRight, t);
	segment_segment_intersection(currBot->coord, d, battleBox.bottomRight, {battleBox.topLeft.x,battleBox.bottomRight.y}, t);
	segment_segment_intersection(currBot->coord, d, {battleBox.topLeft.x,battleBox.bottomRight.y}, battleBox.topLeft, t);

	if(t.size()) data = 100 - *t.begin() * 100;
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
