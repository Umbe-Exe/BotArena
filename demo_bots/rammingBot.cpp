#include "..\src\competition.h"
#include "rammingBot.h"

int R = 0;
int L = 0;

void actions(double delta) {

	int l = getSensorData(L);
	int r = getSensorData(R);

	if(l == 1 && r == 1) setMotorSpeed(100, 100);
	else if(l == 1) {
		setMotorSpeed(-100, 100);
	}
	else if(r == 1) {
		setMotorSpeed(100, -100);
	}
	else setMotorSpeed(50, 100);
}

void ok() {

	setSystemChargeRate(SHIELDS, 100);

	R = addRadarGetId(150 / 2.f, 150, 100);
	L = addRadarGetId(-150 / 2.f, 150, 100);

	setMotorSpeed(50, 100);
}

void registerRam() {
	registerBot("rammer", RANDCOL, 0, actions, ok);
}
