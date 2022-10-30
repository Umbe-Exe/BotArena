#pragma once
#include "../src/competition.h"

class Rammer : public Controller {
	int R = 0;
	int L = 0;

	void onUpdate(double delta) {

		int l = getSensorData(L);
		int r = getSensorData(R);

		if(l == 1 && r == 1) setMotorSpeed(100, 100);
		else if(l == 1) {
			setMotorSpeed(-100, 100);
		} else if(r == 1) {
			setMotorSpeed(100, -100);
		} else setMotorSpeed(50, 100);
	}

	void onStart() {
		setSystemChargeRate(SHIELDS, 100);

		R = addRadarGetId(150 / 2.f, 150, 100);
		L = addRadarGetId(-150 / 2.f, 150, 100);

		setMotorSpeed(50, 100);
	}

	void onDeath(){}
	const char *name() {
		return "Rammer";
	}
	const char *pathToImage() {
		return 0;
	}
	COLOR color() {
		return RANDCOL;
	}
};