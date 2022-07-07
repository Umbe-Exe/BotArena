#include "competition.h"
#include "data.h"
#include "utils.h"

#include <math.h>

int addRadarGetId(int angle, int width, int range) {

	if(width < 0) width *= -1;
	if(range < 0) {
		range *= -1;
		angle += 180;
	}
	
	currBot->sensor = (Sensor **)realloc(currBot->sensor, sizeof(Sensor *) * (currBot->nOfSensors + 1));

	currBot->sensor[currBot->nOfSensors] = new Radar(angle, width, range > 100 ? 100 : range, currBot->color);

	++currBot->nOfSensors;

	return currBot->nOfSensors - 1;
}

int addRangeGetId(int angle, int range) {

	if(range < 0) {
		range *= -1;
		angle += 180;
	}
	
	currBot->sensor = (Sensor **)realloc(currBot->sensor, sizeof(Sensor*) * (currBot->nOfSensors + 1));

	currBot->sensor[currBot->nOfSensors] = new LaserRange(angle, range > 100 ? 100 : range, currBot->color);

	++currBot->nOfSensors;

	return currBot->nOfSensors - 1;
}

void setSensorStatus(int sensorId, bool enabled) {

	if(currBot->nOfSensors >= sensorId && sensorId >= 0)
		currBot->sensor[sensorId]->enabled = 0;
}

int getSensorData(int sensorId) {

	if(currBot->nOfSensors >= sensorId && sensorId >= 0)
		return currBot->sensor[sensorId]->data;
	else return 0;
}

void setMotorSpeed(int leftTread, int rightTread) {

	if(leftTread > 100) leftTread = 100;
	else if(leftTread < -100) leftTread = -100;
	if(rightTread > 100) rightTread = 100;
	else if(rightTread < -100) rightTread = -100;

	currBot->leftTreadSpeed = leftTread;
	currBot->rightTreadSpeed = rightTread;
}

int getGeneratorOutput() {
	return currBot->energy;
}

int getSystemEnergy(System system) {

	switch(system) {
		case MISSILES:
			return currBot->missile;
		case SHIELDS:
			return currBot->shield;
		case LASERS:
			return currBot->laser;
		default:
			return -1;
	}
}

void setSystemChargeRate(System system, int rate) {

	switch(system) {
		case MISSILES:

			if(currBot->shieldChargeRate + currBot->laserChargeRate + rate > 100) 
				rate = 100 - currBot->shieldChargeRate + currBot->laserChargeRate;

			currBot->missileChargeRate = rate;
			break;
		case SHIELDS:

			if(currBot->missileChargeRate + currBot->laserChargeRate + rate > 100)
				rate = 100 - currBot->missileChargeRate + currBot->laserChargeRate;

			currBot->shieldChargeRate = rate;
			break;
		case LASERS:

			if(currBot->missileChargeRate + currBot->shieldChargeRate + rate > 100)
				rate = 100 - currBot->missileChargeRate + currBot->shieldChargeRate;

			currBot->laserChargeRate = rate;
			break;
	}
}

void fireWeapon(Armament weapon, int heading) {

	weapons = (Weapon **)realloc(weapons, sizeof(Weapon *) * (nOfWeapons + 1));

	switch(weapon) {
		case MISSILE:
			weapons[nOfWeapons] = new Missile();
			weapons[nOfWeapons]->bitmap = missileBitmap;
			break;
		case LASER:
			weapons[nOfWeapons] = new Laser(currBot->laser * laserDamageMoltiplicator);
			weapons[nOfWeapons]->bitmap = laserBitmap;
			break;
	}

	weapons[nOfWeapons]->heading = heading;
	weapons[nOfWeapons]->x = cos(heading * RAD_PER_DEG) * botRadius * 0.005;
	weapons[nOfWeapons]->x = sin(heading * RAD_PER_DEG) * botRadius * 0.005;

	++nOfWeapons;
}

GPSdata getGPSdata() {
	return {currBot->x, currBot->y, currBot->heading};
}

bool isBumping() {
	return currBot->bumping;
}
