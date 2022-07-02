#pragma once

enum System {
	MISSILES, LASERS, SHIELDS
};

enum Armament {
	MISSILE, LASER
};

struct GPSdata {
	float x, y, heading;
};

int addRadarGetId(int angle, int width, int range);

int addRangeGetId(int angle, int range);

void setSensorStatus(int sensorId, bool enabled);

int getSensorData(int sensorId);

void setMotorSpeed(int leftTread, int rightTread);

int getGeneratorOutput();

int getSystemEnergy(System system);

void setSystemChargeRate(System system, int rate);

void fireWeapon(Armament weapon, int heading);

GPSdata getGPSdata();

bool isBumping();