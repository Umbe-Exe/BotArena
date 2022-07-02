#include "competition.h"
#include "data.h"

int addRadarGetId(int angle, int width, int range) {
	
	currBot->sensor = (Sensor **)realloc(currBot->sensor, sizeof(Sensor *) * currBot->nOfSensors);

	if(currBot->sensor) abort();

	currBot->sensor[currBot->nOfSensors] = new Radar(angle, width, range > 100 ? 100 : range, currBot->color);

	++currBot->nOfSensors;

	return currBot->nOfSensors - 1;
}

int addRangeGetId(int angle, int range) {
	
	currBot->sensor = (Sensor **)realloc(currBot->sensor, sizeof(Sensor*) * currBot->nOfSensors);

	if(currBot->sensor) abort();

	currBot->sensor[currBot->nOfSensors] = new LaserRange(angle, range > 100 ? 100 : range, currBot->color);

	++currBot->nOfSensors;

	return currBot->nOfSensors - 1;
}

void setSensorStatus(int sensorId, bool enabled) {

	if(currBot->nOfSensors >= sensorId)
		currBot->sensor[sensorId]->enabled = 0;
}

int getSensorData(int sensorId) {

	if(currBot->nOfSensors >= sensorId)
		return currBot->sensor[sensorId]->data;
	else return INT_MAX;
}

void setMotorSpeed(int leftTread, int rightTread) {
	currBot->leftTreadSpeed = leftTread;
	currBot->rightTreadSpeed = rightTread;
}

int getGeneratorOutput() {
	return currBot->energy;
}

int getSystemEnergy(System system) {

	switch(system) {
		case MISSILES:
			return currBot->missileChargeRate;
		case SHIELDS:
			return currBot->shieldChargeRate;
		case LASERS:
			return currBot->laserChargeRate;
		default:
			return -1;
	}
}

void setSystemChargeRate(System system, int rate) {

	switch(system) {
		case MISSILES:
			currBot->missileChargeRate = rate;
			break;
		case SHIELDS:
			currBot->shieldChargeRate = rate;
			break;
		case LASERS:
			currBot->laserChargeRate = rate;
			break;
	}
}

void fireWeapon(Armament weapon, int heading) {


}

GPSdata getGPSdata() {
	return {currBot->x, currBot->y, currBot->heading};
}

bool isBumping() {
	return currBot->bumping;
}
