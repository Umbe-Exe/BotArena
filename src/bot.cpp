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

	al_draw_arc(radius, radius, radius / 2.f, DEG_PER_RAD * (angle - width / 2), DEG_PER_RAD * (width), {currBot->color.r, currBot->color.g, currBot->color.b, 0.5f}, radius);

	al_set_target_backbuffer(target);
}

void Radar::draw() {

	if(enabled) {
		al_draw_rotated_bitmap(bitmap,
							   al_get_bitmap_width(bitmap) / 2,
							   al_get_bitmap_height(bitmap) / 2,
							   currBot->coord.x * arenaSize, currBot->coord.y * arenaSize, DEG_PER_RAD * (currBot->heading + angle), 0);
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
		start = {cosf((currBot->heading + angle - width / 2) * DEG_PER_RAD), sinf((currBot->heading + angle - width / 2) * DEG_PER_RAD)},
		end = {cosf((currBot->heading + angle + width / 2) * DEG_PER_RAD), sinf((currBot->heading + angle + width / 2) * DEG_PER_RAD)};

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
		Coord pointCoord = {cosf((currBot->heading + angle) * DEG_PER_RAD) * (rangeMaxRange / 100 * (range - data)), sinf((currBot->heading + angle) * DEG_PER_RAD) * (rangeMaxRange / 100 * (range - data))};
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

	Coord d = {cosf((currBot->heading + angle) * DEG_PER_RAD), sinf((currBot->heading + angle) * DEG_PER_RAD)};
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
			       coord.x * arenaSize, coord.y * arenaSize, DEG_PER_RAD * heading, 0);
}

void Bot::update(double delta) {

	currBot = this;

	for(Sensor *sensor : sensors) sensor->update(delta);

	if(updateFn) updateFn(delta);
////////////////////////
////////////////////////MOVEMENT
	float rotAngle, startAngle, lTreadDist, rTreadDist, x, y, innerRad, midRad,
		u, v, radians, dist;
	int lTreadSpeed, rTreadSpeed;

	if(impulseSpeed != 0) {

		radians = impulseHeading * DEG_PER_RAD;
		dist = impulseSpeed * delta;
		coord += {dist *cosf(radians), dist * sinf(radians)};
		impulseSpeed -= friction * delta;
		if(impulseSpeed < 0) impulseSpeed = 0;
	}

	if(turboTime) {

		lTreadSpeed = leftTreadSpeed + 200;
		rTreadSpeed = rightTreadSpeed + 200;
		turboTime--;
	} else {

		lTreadSpeed = leftTreadSpeed;
		rTreadSpeed = rightTreadSpeed;
	}
	lTreadDist = maxSpeed * lTreadSpeed / (100.0 / delta);
	rTreadDist = maxSpeed * rTreadSpeed / (100.0 / delta);

	if(lTreadSpeed == rTreadSpeed) {

		radians = heading * DEG_PER_RAD;
		coord += {lTreadDist * cosf(radians), lTreadDist * sinf(radians)};
	} else {

		if(rTreadSpeed == 0) {

			midRad = botRadius;
			rotAngle = -lTreadDist * 360.0 / (2 * PI * 2 * botRadius);
			startAngle = heading + 90;
		} else if(lTreadSpeed == 0) {

			midRad = botRadius;
			rotAngle = rTreadDist * 360.0 / (2 * PI * 2 * botRadius);
			startAngle = heading + 270;
		} else {

			if(abs(lTreadSpeed) > abs(rTreadSpeed)) {

				innerRad = rTreadDist * 2 * botRadius / (lTreadDist - rTreadDist);
				midRad = innerRad + 2 * botRadius / 2;
				rotAngle = -rTreadDist * 360.0 / (2 * PI * innerRad);
				startAngle = heading + 90;
			} else {
				innerRad = lTreadDist * 2 * botRadius / (rTreadDist - lTreadDist);
				midRad = innerRad + 2 * botRadius / 2;
				rotAngle = lTreadDist * 360.0 / (2 * PI * innerRad);
				startAngle = heading + 270;
			}
		}
		radians = startAngle * DEG_PER_RAD;
		x = midRad * cosf(radians);
		y = midRad * sinf(radians);
		radians = rotAngle * DEG_PER_RAD;
		u = x * cosf(radians) - y * sinf(radians);
		v = y * cosf(radians) + x * sinf(radians);
		coord += {u - x, v - y};
		heading += rotAngle;

		if(heading >= 360) heading -= 360;
		else if(heading < 0) heading += 360;
	}
////////////////////////
////////////////////////BOTS COLLIDING
	float /*x, y, dist,*/ angle;

	for(Bot *bot : bots) {
		if(bot != this) {

			x = coord.x - bot->coord.x;
			y = coord.y - bot->coord.y;
			dist = sqrtf(powf(x, 2) + powf(y, 2));

			if(dist < botRadius * 2) {
				impulseSpeed = bumpForce;
				if(y >= 0) angle = acosf(x / dist);
				else angle = 2 * PI - acosf(x / dist);
				angle *= RAD_PER_DEG;
				impulseHeading = angle;
				bumping = 1;

				if(shield > shieldLeakLevel) shield -= bumpDamage;
				else {
					uint8_t generatorDamage = bumpDamage - bumpDamage / 100 * shield;
					shield -= bumpDamage - generatorDamage;
					energy -= generatorDamage;
				}
			}
		}
	}
////////////////////////
////////////////////////WALL HIT
	bool hitWall = 0;
	if(coord.x < battleBox.topLeft.x + botRadius) {
		coord.x = battleBox.topLeft.x + botRadius;
		hitWall = 1;
	} else if(coord.x > battleBox.bottomRight.x - botRadius) {
		coord.x = battleBox.bottomRight.x - botRadius;
		hitWall = 1;
	}

	if(coord.y < battleBox.topLeft.y + botRadius) {
		coord.y = battleBox.topLeft.y + botRadius;
		hitWall = 1;
	} else if(coord.y > battleBox.bottomRight.y - botRadius) {
		coord.y = battleBox.bottomRight.y - botRadius;
		hitWall = 1;
	}
	if(hitWall) bumping = 1;
}
