#include "arena_impl.h"
#include "common.h"

void Radar::priming() {

	if(bitmap) al_destroy_bitmap(bitmap);

	float radius = arena.radarMaxRange / 100 * range * arena.arenaSize;

	bitmap = al_create_bitmap(radius * 2, radius * 2);

	al_set_target_bitmap(bitmap);

	al_draw_arc(radius, radius, radius / 2.f, DEG_PER_RAD * (angle - width / 2), DEG_PER_RAD * (width), {arena.currBot->color.r, arena.currBot->color.g, arena.currBot->color.b, 0.5f}, radius);

	al_set_target_backbuffer(arena.window);
}

void Radar::draw() {

	if(enabled)
		al_draw_rotated_bitmap(bitmap,
							   al_get_bitmap_width(bitmap) / 2,
							   al_get_bitmap_height(bitmap) / 2,
							   arena.currBot->coord.x * arena.arenaSize, arena.currBot->coord.y * arena.arenaSize, DEG_PER_RAD * arena.currBot->heading, 0);
}

void Radar::update(double delta) {

	if(enabled) {
		data = 0;

		Coord
			start = {cosf((arena.currBot->heading + angle - width / 2) * DEG_PER_RAD), sinf((arena.currBot->heading + angle - width / 2) * DEG_PER_RAD) },
			end = {cosf((arena.currBot->heading + angle + width / 2) * DEG_PER_RAD), sinf((arena.currBot->heading + angle + width / 2) * DEG_PER_RAD) };

		float startColl, endColl;

		for(Bot &bot : arena.bots)
			if(&bot != arena.currBot) {

				startColl = segmentCircleIntersection(arena.botRadius, arena.currBot->coord, start, bot.coord);
				endColl = segmentCircleIntersection(arena.botRadius, arena.currBot->coord, end, bot.coord);

				if(getDistance(bot.coord, arena.currBot->coord) < arena.botRadius + arena.radarMaxRange / 100 * range && (
					(startColl > 0 && startColl / (arena.radarMaxRange / 100 * range) < 1.f) ||
					(endColl > 0 && endColl / (arena.radarMaxRange / 100 * range) < 1.f) || (
						circleCircleSectorIntersection(arena.currBot->coord, start, bot.coord, end)))) {
					data = 1;
					break;
				}
			}
	}
}

void LaserRange::draw() {

	if(enabled) {
		Coord pointCoord = {cosf((arena.currBot->heading + angle) * DEG_PER_RAD) * (arena.rangeMaxRange / 100 * (range - data)), sinf((arena.currBot->heading + angle) * DEG_PER_RAD) * (arena.rangeMaxRange / 100 * (range - data))};
		pointCoord += arena.currBot->coord;

		al_draw_line(arena.currBot->coord.x * arena.arenaSize, arena.currBot->coord.y * arena.arenaSize, pointCoord.x * arena.arenaSize, pointCoord.y * arena.arenaSize,
					 {arena.currBot->color.r, arena.currBot->color.g, arena.currBot->color.b, 1.f}, 0.003f * arena.arenaSize);

		al_draw_filled_circle(pointCoord.x * arena.arenaSize, pointCoord.y * arena.arenaSize, 0.005f * arena.arenaSize, {arena.currBot->color.r, arena.currBot->color.g, arena.currBot->color.b, 1.f});
	}
}

void LaserRange::update(double delta) {

	if(enabled) {
		data = 0;

		std::set<float> t;

		Coord d = {cosf((arena.currBot->heading + angle) * DEG_PER_RAD), sinf((arena.currBot->heading + angle) * DEG_PER_RAD)};
		float tmp;

		for(Bot &bot : arena.bots)
			if(&bot != arena.currBot) {

				if((tmp = segmentCircleIntersection(arena.botRadius, arena.currBot->coord, d, bot.coord)) == 0) continue;

				tmp /= (arena.rangeMaxRange / 100 * range);

				if(tmp < 1.f) t.insert(tmp);
			}

		d *= arena.rangeMaxRange / 100 * range;
		d += arena.currBot->coord;

		segment_segment_intersection(arena.currBot->coord, d, arena.battleBox.topLeft, {arena.battleBox.bottomRight.x,arena.battleBox.topLeft.y}, t);
		segment_segment_intersection(arena.currBot->coord, d, {arena.battleBox.bottomRight.x,arena.battleBox.topLeft.y}, arena.battleBox.bottomRight, t);
		segment_segment_intersection(arena.currBot->coord, d, arena.battleBox.bottomRight, {arena.battleBox.topLeft.x,arena.battleBox.bottomRight.y}, t);
		segment_segment_intersection(arena.currBot->coord, d, {arena.battleBox.topLeft.x,arena.battleBox.bottomRight.y}, arena.battleBox.topLeft, t);

		if(t.size()) data = range - *t.begin() * range;
	}
}

void Bot::draw() {

	arena.currBot = this;

	for(Sensor *sensor : sensors) sensor->draw();

	al_draw_rotated_bitmap(bitmap,
			       al_get_bitmap_width(bitmap) / 2,
			       al_get_bitmap_height(bitmap) / 2,
			       coord.x * arena.arenaSize, coord.y * arena.arenaSize, DEG_PER_RAD * heading, 0);
}

void Bot::update(double delta) {
	
	arena.currBot = this;

	for(Sensor *sensor : sensors) sensor->update(delta);

	controller.onUpdate(delta);

	bumping = 0;
////////////////////////
////////////////////////ENERGY HANDLING
	uint8_t sensorsDraw = 0;
	for(Sensor *sensor : sensors)
		if(sensor->enabled) sensorsDraw += arena.sensorEnergyConsumption;

	sensorsDraw -= (100 - (shieldChargeRate + missileChargeRate + laserChargeRate));

	shield += generator / arena.maxGeneratorStructure * arena.maxEnergy / 100 * (shieldChargeRate - sensorsDraw / 100 * shieldChargeRate) * delta;
	missile += generator / arena.maxGeneratorStructure * arena.maxEnergy / 100 * (missileChargeRate - sensorsDraw / 100 * missileChargeRate) * delta;
	laser += generator / arena.maxGeneratorStructure * arena.maxEnergy / 100 * (laserChargeRate - sensorsDraw / 100 * laserChargeRate) * delta;
	if(shield > arena.maxShield) shield = arena.maxShield;
	if(missile > arena.maxMissile) missile = arena.maxMissile;
	if(laser > arena.maxLaser) laser = arena.maxLaser;
////////////////////////
////////////////////////MOVEMENT
	float rotAngle, startAngle, lTreadDist, rTreadDist, x, y, innerRad, midRad,
		u, v, radians, dist;

	if(impulseSpeed != 0) {

		radians = impulseHeading * DEG_PER_RAD;
		dist = impulseSpeed * delta;
		coord += {dist *cosf(radians), dist *sinf(radians)};
		impulseSpeed -= arena.friction * delta;
		if(impulseSpeed < 0) impulseSpeed = 0;
	}

	lTreadDist = arena.maxSpeed * leftTreadSpeed / (100.0 / delta);
	rTreadDist = arena.maxSpeed * rightTreadSpeed / (100.0 / delta);

	if(leftTreadSpeed == rightTreadSpeed) {

		radians = heading * DEG_PER_RAD;
		coord += {lTreadDist *cosf(radians), lTreadDist *sinf(radians)};
	} else {

		if(leftTreadSpeed == 0) {

			midRad = arena.botRadius;
			rotAngle = -rTreadDist * 360.0 / (2 * PI * 2 * arena.botRadius);
			startAngle = heading + 90;
		} else if(rightTreadSpeed == 0) {

			midRad = arena.botRadius;
			rotAngle = lTreadDist * 360.0 / (2 * PI * 2 * arena.botRadius);
			startAngle = heading + 270;
		} else {
			if(abs(leftTreadSpeed) > abs(rightTreadSpeed)) {

				innerRad = rTreadDist * 2 * arena.botRadius / (lTreadDist - rTreadDist);
				midRad = innerRad + 2 * arena.botRadius / 2;
				rotAngle = rTreadDist * 360.0 / (2 * PI * innerRad);
				startAngle = heading - 90;
			} else {
				innerRad = lTreadDist * 2 * arena.botRadius / (rTreadDist - lTreadDist);
				midRad = innerRad + 2 * arena.botRadius / 2;
				rotAngle = -lTreadDist * 360.0 / (2 * PI * innerRad);
				startAngle = heading - 270;
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
	}
////////////////////////
////////////////////////BOTS COLLIDING
	float /*x, y, dist,*/ angle;
	uint8_t i = (this - arena.bots.data() + 1);

	for(; i < arena.bots.size(); ++i) {

		x = coord.x - arena.bots[i].coord.x;
		y = coord.y - arena.bots[i].coord.y;
		dist = sqrtf(powf(x, 2) + powf(y, 2));

		if(dist < arena.botRadius * 2) {
			impulseSpeed = arena.bumpForce;
			arena.bots[i].impulseSpeed = arena.bumpForce;
			if(y >= 0) angle = acosf(x / dist);
			else angle = 2 * PI - acosf(x / dist);
			angle *= RAD_PER_DEG;
			impulseHeading = angle;
			arena.bots[i].impulseHeading = angle + 180;
			bumping |= BotCollision;
			arena.bots[i].bumping |= BotCollision;

			if(shield > arena.shieldLeakLevel) shield -= arena.bumpDamage;
			else {
				uint8_t generatorDamage = arena.bumpDamage - arena.bumpDamage / arena.maxShield * shield;
				shield -= arena.bumpDamage + generatorDamage;
				generator -= generatorDamage;
			}

			if(shield < 0) shield = 0;
			if(generator <= 0) arena.addBotToDestroy(this);

			if(arena.bots[i].shield > arena.shieldLeakLevel) arena.bots[i].shield -= arena.bumpDamage;
			else {
				uint8_t generatorDamage = arena.bumpDamage - arena.bumpDamage / arena.maxShield * arena.bots[i].shield;
				arena.bots[i].shield -= arena.bumpDamage + generatorDamage;
				arena.bots[i].generator -= generatorDamage;
			}

			if(arena.bots[i].shield < 0) arena.bots[i].shield = 0;
			if(arena.bots[i].generator <= 0) arena.addBotToDestroy(&arena.bots[i]);

#ifdef SOUND
			if(arena.allowFeedback && arena.allowSound)
				playSound(SoundType::BOTCOLLISION);
#endif
#ifdef PARTICLES
			if(arena.allowFeedback && arena.allowParticles) {
				auto &vec = getParticleData(ParticleType::BOTCOLLISION);

				arena.particles.reserve(arena.particles.size() + vec.size());

				for(auto &particle : vec) arena.particles.emplace_back(
					x / 2.f + arena.bots[i].coord.x, y / 2.f + arena.bots[i].coord.y, particle.color, particle.heading, particle.speed,
					particle.timeToLive);
			}
#endif
		}
	}
////////////////////////
////////////////////////WALL HIT
	if(coord.x < arena.battleBox.topLeft.x + arena.botRadius) {
		coord.x = arena.battleBox.topLeft.x + arena.botRadius;
		bumping |= WallHit;
	} else if(coord.x > arena.battleBox.bottomRight.x - arena.botRadius) {
		coord.x = arena.battleBox.bottomRight.x - arena.botRadius;
		bumping |= WallHit;
	}

	if(coord.y < arena.battleBox.topLeft.y + arena.botRadius) {
		coord.y = arena.battleBox.topLeft.y + arena.botRadius;
		bumping |= WallHit;
	} else if(coord.y > arena.battleBox.bottomRight.y - arena.botRadius) {
		coord.y = arena.battleBox.bottomRight.y - arena.botRadius;
		bumping |= WallHit;
	}
}

Bot &Bot::operator=(const Bot &bot) {
	arena = bot.arena;
	generator = bot.generator;
	shield = bot.shield;
	missile = bot.missile;
	laser = bot.laser;
	name = bot.name;
	image = bot.image;
	color = bot.color;
	controller = bot.controller;
	coord = bot.coord;
	heading = bot.heading;
	leftTreadSpeed = bot.leftTreadSpeed;
	rightTreadSpeed = bot.rightTreadSpeed;
	shieldChargeRate = bot.shieldChargeRate;
	missileChargeRate = bot.missileChargeRate;
	laserChargeRate = bot.laserChargeRate;
	bumping = bot.bumping;
	impulseSpeed = bot.impulseSpeed;
	impulseHeading = bot.impulseHeading;

	bitmap = al_clone_bitmap(bot.bitmap);
	sensors.clear();
	Bot *tmp = arena.currBot;
	arena.currBot = this;
	for(Sensor *sensor : bot.sensors) sensors.push_back(sensor->clone());
	arena.currBot = tmp;

	return *this;
}