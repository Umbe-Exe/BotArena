#include "..\src\competition.h"
#include "Umbe4.h"
#include <time.h>

namespace umbe4 {

	int RRADAR = 0;
	int LRADAR = 0;
	int RRANGE = 0;
	int LRANGE = 0;

#define R 0
#define L 1
#define FORWARD 3

#define DELTA(x) (float)(clock()-x)/CLOCKS_PER_SEC

	int UM_gc[799], UM_s = 0, UM_p = 0; //GOSPER-CURVE NODES, SIZE, POSITION READ OR ABOUT TO BE READ
	clock_t UM_time = 0; //USED TO GIVE PACE TO THE ROAMING AND TO MAKE TURNS ABOUT 60DEG
	clock_t UM_delay = 0; //USED TO DELAY ENGAGING AND COME BACK INTO ENEMY SIGHT
	int UM_L = 0, UM_R = 0, UM_fl = 0, UM_fr = 0; //LEFT RADAR DATA, RIGHT RADAR DATA, LEFT RANGE DATA, RIGHT RANGE DATA
	//UM_L AND UM_R ARE USED AS BOOLEANS
	//f STANDS FOR 'FAR', I JUST KEPT THE NAME FROM A PREVIOUS BOT CONFIGURATION

	int UM_hit = 0;
	float UM_t = .5f;

	void manageEnergy() {

		int shield = getSystemEnergy(SHIELDS)
			, missle = getSystemEnergy(MISSILES)
			, laser = getSystemEnergy(LASERS)
			, generator = getGeneratorOutput();

		if(generator < 60) {
			if(missle == 100 && laser == 50) {
				setSystemChargeRate(SHIELDS, 100);
				setSystemChargeRate(MISSILES, 0);
				setSystemChargeRate(LASERS, 0);
			} else if(missle == 100) {
				setSystemChargeRate(SHIELDS, 40);
				setSystemChargeRate(MISSILES, 0);
				setSystemChargeRate(LASERS, 60);
			} else if(laser == 100) {
				setSystemChargeRate(SHIELDS, 40);
				setSystemChargeRate(MISSILES, 60);
				setSystemChargeRate(LASERS, 0);
			} else {
				setSystemChargeRate(SHIELDS, 20);
				setSystemChargeRate(MISSILES, 0);
				setSystemChargeRate(LASERS, 80);
			}
		} else if(shield == 100) {
			if(missle == 100 && laser == 100) {
				setSystemChargeRate(SHIELDS, 0);
				setSystemChargeRate(MISSILES, 0);
				setSystemChargeRate(LASERS, 0);
			} else if(missle == 100) {
				setSystemChargeRate(SHIELDS, 0);
				setSystemChargeRate(MISSILES, 0);
				setSystemChargeRate(LASERS, 100);
			} else if(laser == 100) {
				setSystemChargeRate(SHIELDS, 0);
				setSystemChargeRate(MISSILES, 100);
				setSystemChargeRate(LASERS, 0);
			} else {
				setSystemChargeRate(SHIELDS, 0);
				setSystemChargeRate(MISSILES, 60);
				setSystemChargeRate(LASERS, 40);
			}
		} else if(shield > 80) {
			if(missle == 100 && laser == 100) {
				setSystemChargeRate(SHIELDS, 100);
				setSystemChargeRate(MISSILES, 0);
				setSystemChargeRate(LASERS, 0);
			} else if(missle == 100) {
				setSystemChargeRate(SHIELDS, 40);
				setSystemChargeRate(MISSILES, 0);
				setSystemChargeRate(LASERS, 60);
			} else if(laser == 100) {
				setSystemChargeRate(SHIELDS, 40);
				setSystemChargeRate(MISSILES, 60);
				setSystemChargeRate(LASERS, 0);
			} else {
				setSystemChargeRate(SHIELDS, 20);
				setSystemChargeRate(MISSILES, 40);
				setSystemChargeRate(LASERS, 40);
			}
		} else if(missle == 100 && laser == 100) {
			setSystemChargeRate(SHIELDS, 100);
			setSystemChargeRate(MISSILES, 0);
			setSystemChargeRate(LASERS, 0);
		} else if(missle == 100) {
			setSystemChargeRate(SHIELDS, 60);
			setSystemChargeRate(MISSILES, 0);
			setSystemChargeRate(LASERS, 40);
		} else if(laser == 100) {
			setSystemChargeRate(SHIELDS, 50);
			setSystemChargeRate(MISSILES, 50);
			setSystemChargeRate(LASERS, 0);
		} else {
			setSystemChargeRate(SHIELDS, 30);
			setSystemChargeRate(MISSILES, 40);
			setSystemChargeRate(LASERS, 30);
		}

		if(UM_L || UM_R || shield < 60 ||
		   UM_t == 5.f || UM_hit > shield ||
		   (missle < 80 && laser < 80) ||
		   DELTA(UM_delay) < 5.f) {
			setSensorStatus(RRANGE, 0);
			setSensorStatus(LRANGE, 0);
		} else {
			setSensorStatus(RRANGE, 1);
			setSensorStatus(LRANGE, 1);
		}

		if(DELTA(UM_delay) < .3f) {
			setSensorStatus(RRADAR, 0);
			setSensorStatus(LRADAR, 0);
		} else {
			setSensorStatus(RRADAR, 1);
			setSensorStatus(LRADAR, 1);
		}

	}

	void roam() {

		if(UM_p == UM_s) UM_p = 0;

		if(UM_hit > getSystemEnergy(SHIELDS)) {
			UM_t = 5.f;
			UM_p = 0;
		}

		switch(UM_gc[UM_p]) {
			case R:
				setMotorSpeed(100, -100);
				break;
			case L:
				setMotorSpeed(-100, 100);
				break;
			case FORWARD:
				setMotorSpeed(100, 100);
				break;
		}

		switch(UM_gc[UM_p]) {
			case R:
			case L:
				if(DELTA(UM_time) > UM_t) {
					UM_p++;
					UM_time = clock();
					UM_t = .5f;
				}
				break;
			case FORWARD:
				if(DELTA(UM_time) > .4f) {
					UM_p++;
					UM_time = clock();
				}
				break;
		}
	}

	void rg(int st);

	void gl(int st) {
		st -= 1;

		if(st > 0) {
			UM_gc[UM_s++] = L;
			rg(st);
			UM_gc[UM_s++] = R;
			gl(st);
			gl(st);
			UM_gc[UM_s++] = R;
			UM_gc[UM_s++] = R;
			gl(st);
			UM_gc[UM_s++] = R;
			rg(st);
			UM_gc[UM_s++] = L;
			UM_gc[UM_s++] = L;
			rg(st);
			UM_gc[UM_s++] = L;
			gl(st);
		}
		if(st == 0) {
			UM_gc[UM_s++] = L;
			UM_gc[UM_s++] = FORWARD;
			UM_gc[UM_s++] = R;
			UM_gc[UM_s++] = FORWARD;
			UM_gc[UM_s++] = FORWARD;
			UM_gc[UM_s++] = R;
			UM_gc[UM_s++] = R;
			UM_gc[UM_s++] = FORWARD;
			UM_gc[UM_s++] = R;
			UM_gc[UM_s++] = FORWARD;
			UM_gc[UM_s++] = L;
			UM_gc[UM_s++] = L;
			UM_gc[UM_s++] = FORWARD;
			UM_gc[UM_s++] = L;
			UM_gc[UM_s++] = FORWARD;
		}
	}

	void rg(int st) {
		st -= 1;

		if(st > 0) {
			rg(st);
			UM_gc[UM_s++] = R;
			gl(st);
			UM_gc[UM_s++] = R;
			UM_gc[UM_s++] = R;
			gl(st);
			UM_gc[UM_s++] = L;
			rg(st);
			UM_gc[UM_s++] = L;
			UM_gc[UM_s++] = L;
			rg(st);
			rg(st);
			UM_gc[UM_s++] = L;
			gl(st);
			UM_gc[UM_s++] = R;
		}
		if(st == 0) {
			UM_gc[UM_s++] = FORWARD;
			UM_gc[UM_s++] = R;
			UM_gc[UM_s++] = FORWARD;
			UM_gc[UM_s++] = R;
			UM_gc[UM_s++] = R;
			UM_gc[UM_s++] = FORWARD;
			UM_gc[UM_s++] = L;
			UM_gc[UM_s++] = FORWARD;
			UM_gc[UM_s++] = L;
			UM_gc[UM_s++] = L;
			UM_gc[UM_s++] = FORWARD;
			UM_gc[UM_s++] = FORWARD;
			UM_gc[UM_s++] = L;
			UM_gc[UM_s++] = FORWARD;
			UM_gc[UM_s++] = R;
		}
	}

	void UmbeActions(double delta) {

		UM_L = getSensorData(LRADAR);
		UM_R = getSensorData(RRADAR);

		manageEnergy();

		UM_fl = getSensorData(LRANGE);
		UM_fr = getSensorData(RRANGE);

		if(UM_L == 1 && UM_R == 1) {
			if(getSystemEnergy(MISSILES) == 100) fireWeapon(MISSILE, 0);
			if(getSystemEnergy(LASERS) > 40) fireWeapon(LASER, 0);
			setMotorSpeed(-100, -100);

			UM_delay = clock();

		} else if(UM_L == -1 || UM_R == -1) setMotorSpeed(-100, -100);
		else if(UM_L == 0 && UM_R == 1) {
			setMotorSpeed(40, -39);
			if(DELTA(UM_delay) > 1.f && DELTA(UM_delay) < 10.f) UM_delay += CLOCKS_PER_SEC;
		} else if(UM_L == 1 && UM_R == 0) {
			setMotorSpeed(-39, 40);
			if(DELTA(UM_delay) > 1.f && DELTA(UM_delay) < 10.f) UM_delay += CLOCKS_PER_SEC;
		} else if((UM_fl > 0 || UM_fr > 0) && UM_fl > UM_fr) {
			setMotorSpeed(100, 20);
			if(isBumping()) setMotorSpeed(50, -100);
		} else if((UM_fl > 0 || UM_fr > 0) && UM_fl < UM_fr) {
			setMotorSpeed(20, 100);
			if(isBumping()) setMotorSpeed(50, -100);
		} else if(DELTA(UM_delay) > 1.f && DELTA(UM_delay) < 5.f) {
			setMotorSpeed(100, 100);
			if(UM_hit > getSystemEnergy(SHIELDS)) {
				UM_delay -= 5;
				roam();
			}
		} else if(DELTA(UM_delay) < 1.f) {
			if(getSystemEnergy(MISSILES) == 100.f || getSystemEnergy(LASERS) > 40.f) UM_delay -= CLOCKS_PER_SEC;
		} else roam();

		UM_hit = getSystemEnergy(SHIELDS);

	}

	void ready() {

		RRADAR = addRadarGetId(45 / 2.f + 2, 45, 100);
		LRADAR = addRadarGetId(-45 / 2.f - 2, 45, 100);
		RRANGE = addRangeGetId(43, 100);
		LRANGE = addRangeGetId(317, 100);

		gl(3); //gosper curve generation
		UM_time = clock();
		UM_delay = clock() - 10 * CLOCKS_PER_SEC; //to avoid forward chase at the start

	}
}
void registerUmbe4() {
	registerBot("Umbe4", RANDCOL, "resources/images/Coke.bmp", umbe4::UmbeActions, umbe4::ready);
}