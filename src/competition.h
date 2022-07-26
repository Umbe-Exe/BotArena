#pragma once

enum System {
	MISSILES, LASERS, SHIELDS
};

enum Armament {
	MISSILE, LASER
};

enum BumpType {
	WallHit = 1,
	BotCollision = 2,
	MissileHit = 4,
	LaserHit = 8,
	BlastWave = 16
};

struct GPSdata {
	float x, y, heading;
};

int addRadarGetId(float angle, float width, int range);

int addRangeGetId(float angle, int range);

void setSensorStatus(int sensorId, bool enabled);

int getSensorData(int sensorId);

void setMotorSpeed(int leftTread, int rightTread);

int getGeneratorOutput();

int getSystemEnergy(System system);

void setSystemChargeRate(System system, int rate);

void fireWeapon(Armament weapon, float heading);

GPSdata getGPSdata();

bool isBumping();