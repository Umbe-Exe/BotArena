#include "competition.h"
#include "weapon.h"
#include "utils.h"
#include <cmath>

int Controller::addRadarGetId(float angle, float width, float range) {

	if(range < 0) {
		range *= -1;
		angle += 180;
	}

	if(angle > 360) angle -= int(angle / 360) * 360;
	else if(angle < 0) angle += int(angle / -360) * 360;
	if(width > 360) width -= int(width / 360) * 360;
	else if(width < 360) width += int(width / -360) * 360;
	
	arena->currBot->sensors.push_back(new Radar(*arena, width, angle, range > 100 ? 100 : range));

	return (int)arena->currBot->sensors.size() - 1;
}

int Controller::addRangeGetId(float angle, float range) {

	if(range < 0) {
		range *= -1;
		angle += 180;
	}

	if(angle > 360) angle -= int(angle / 360) * 360;
	else if(angle < 0) angle += int(angle / -360) * 360;
	
	arena->currBot->sensors.push_back(new LaserRange(*arena, angle, range > 100 ? 100 : range));

	return (int)arena->currBot->sensors.size() - 1;
}

void Controller::setSensorStatus(int sensorId, bool enabled) {

	if((int)arena->currBot->sensors.size() > sensorId && sensorId >= 0)
		arena->currBot->sensors[sensorId]->enabled = enabled;
}

int Controller::getSensorData(int sensorId) {

	if((int)arena->currBot->sensors.size() > sensorId && sensorId >= 0)
		if(arena->currBot->sensors[sensorId]->enabled)
			return arena->currBot->sensors[sensorId]->data;
	return -1;
}

void Controller::setMotorSpeed(float leftTread, float rightTread) {

	if(leftTread > 100) leftTread = 100;
	else if(leftTread < -100) leftTread = -100;
	if(rightTread > 100) rightTread = 100;
	else if(rightTread < -100) rightTread = -100;

	arena->currBot->leftTreadSpeed = leftTread;
	arena->currBot->rightTreadSpeed = rightTread;
}

int Controller::getGeneratorOutput() {
	return arena->currBot->generator / arena->maxGeneratorStructure * 100;
}

int Controller::getSystemEnergy(System system) {

	switch(system) {
		case MISSILES:
			return arena->currBot->missile / arena->maxMissile * 100;
		case SHIELDS:
			return arena->currBot->shield / arena->maxShield * 100;
		case LASERS:
			return arena->currBot->laser / arena->maxLaser * 100;
	} return 0;
}

void Controller::setSystemChargeRate(System system, float rate) {

	switch(system) {
		case MISSILES:
			if(arena->currBot->shieldChargeRate + arena->currBot->laserChargeRate + rate > 100)
				rate = 100 - (arena->currBot->shieldChargeRate + arena->currBot->laserChargeRate);

			arena->currBot->missileChargeRate = rate;
			break;
		case SHIELDS:
			if(arena->currBot->missileChargeRate + arena->currBot->laserChargeRate + rate > 100)
				rate = 100 - (arena->currBot->missileChargeRate + arena->currBot->laserChargeRate);

			arena->currBot->shieldChargeRate = rate;
			break;
		case LASERS:
			if(arena->currBot->missileChargeRate + arena->currBot->shieldChargeRate + rate > 100)
				rate = 100 - (arena->currBot->missileChargeRate + arena->currBot->shieldChargeRate);

			arena->currBot->laserChargeRate = rate;
			break;
	}
}

void Controller::fireWeapon(Armament weapon, float heading) {

	heading += arena->currBot->heading;
	Coord coord = arena->currBot->coord + Coord{cosf(heading * DEG_PER_RAD) * (arena->botRadius + arena->weaponRadius * 1.1f), sinf(heading * DEG_PER_RAD) * (arena->botRadius + arena->weaponRadius * 1.1f)};

	switch(weapon) {
		case MISSILE:
			if(arena->currBot->missile == arena->maxMissile) {
				arena->weapons.push_back(new Missile(*arena, heading, coord));
				arena->currBot->missile = 0;

#ifdef SOUND
				playSound(SoundType::MISSILE);
#endif
			}
			break;
		case LASER:
			if(arena->currBot->laser >= arena->minLaser) {
				arena->weapons.push_back(new Laser(*arena, arena->currBot->laser * arena->laserDamageMoltiplicator, heading, coord));
				arena->currBot->laser = 0;

#ifdef SOUND
				playSound(SoundType::LASER);
#endif
			}
			break;
	}
}

GPSdata Controller::getGPSdata() {
	return {arena->currBot->coord.x - arena->battleBox.topLeft.x, arena->currBot->coord.y - arena->battleBox.topLeft.y, arena->currBot->heading};
}

char Controller::isBumping() {
	return arena->currBot->bumping;
}
