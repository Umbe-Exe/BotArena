#pragma once

enum SensorType {
	RADAR,
	RANGE
};

int getNewSensorId(SensorType type, int angle, int width, int range);