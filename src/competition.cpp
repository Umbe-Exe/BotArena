#include "competition.h"
#include "data.h"
#include "utils.h"

int addRadarGetId(int angle, int width, int range) {

	if(width < 0) width *= -1;
	if(range < 0) {
		range *= -1;
		angle += 180;
	}

	angle %= 360;
	width %= 360;
	
	currBot->sensors.push_back(new Radar(angle, width, range > 100 ? 100 : range, currBot->color));

	return currBot->sensors.size() - 1;
}

int addRangeGetId(int angle, int range) {

	if(range < 0) {
		range *= -1;
		angle += 180;
	}

	angle %= 360;
	
	currBot->sensors.push_back(new LaserRange(angle, range > 100 ? 100 : range, currBot->color));

	return currBot->sensors.size() - 1;
}

void setSensorStatus(int sensorId, bool enabled) {

	if(currBot->sensors.size() > sensorId && sensorId >= 0)
		currBot->sensors[sensorId]->enabled = enabled;
}

int getSensorData(int sensorId) {

	if(currBot->sensors.size() > sensorId && sensorId >= 0)
		return currBot->sensors[sensorId]->data;
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

	weapons.push_back(0);

	switch(weapon) {
		case MISSILE:
			weapons.back() = new Missile();
			break;
		case LASER:
			weapons.back() = new Laser(currBot->laser * laserDamageMoltiplicator);
			break;
	}

	weapons.back()->heading = heading;
	weapons.back()->coord = {(float)cos(heading * RAD_PER_DEG) * (botRadius + 0.005f), (float)sin(heading * RAD_PER_DEG) * (botRadius + 0.005f)};
}

GPSdata getGPSdata() {
	return {currBot->coord.x, currBot->coord.y, currBot->heading};
}

bool isBumping() {
	return currBot->bumping;
}
