#include "test_bot.h"

void test_actions(double delta) {

	static double elapsed = 0;

	elapsed += delta;

	if((int)elapsed % 50 == 0)
		fireWeapon(MISSILE, 50);
}

void test_init() {

	addRangeGetId(0, 100);
	addRadarGetId(0, 90, 100);

	setMotorSpeed(70, 100);
}
