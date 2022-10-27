#include "..\src\competition.h"
#include "Umbe.h"
#include <cmath>

#define R 0
#define L 1
#define FORWARD 3
#define delta(x) elapsed-x

void Umbe::gl(int st) {
	st -= 1;

	if(st > 0) {
		gc[s++] = L;
		rg(st);
		gc[s++] = R;
		gl(st);
		gl(st);
		gc[s++] = R;
		gc[s++] = R;
		gl(st);
		gc[s++] = R;
		rg(st);
		gc[s++] = L;
		gc[s++] = L;
		rg(st);
		gc[s++] = L;
		gl(st);
	}
	if(st == 0) {
		gc[s++] = L;
		gc[s++] = FORWARD;
		gc[s++] = R;
		gc[s++] = FORWARD;
		gc[s++] = FORWARD;
		gc[s++] = R;
		gc[s++] = R;
		gc[s++] = FORWARD;
		gc[s++] = R;
		gc[s++] = FORWARD;
		gc[s++] = L;
		gc[s++] = L;
		gc[s++] = FORWARD;
		gc[s++] = L;
		gc[s++] = FORWARD;
	}
}

void Umbe::rg(int st) {
	st -= 1;

	if(st > 0) {
		rg(st);
		gc[s++] = R;
		gl(st);
		gc[s++] = R;
		gc[s++] = R;
		gl(st);
		gc[s++] = L;
		rg(st);
		gc[s++] = L;
		gc[s++] = L;
		rg(st);
		rg(st);
		gc[s++] = L;
		gl(st);
		gc[s++] = R;
	}
	if(st == 0) {
		gc[s++] = FORWARD;
		gc[s++] = R;
		gc[s++] = FORWARD;
		gc[s++] = R;
		gc[s++] = R;
		gc[s++] = FORWARD;
		gc[s++] = L;
		gc[s++] = FORWARD;
		gc[s++] = L;
		gc[s++] = L;
		gc[s++] = FORWARD;
		gc[s++] = FORWARD;
		gc[s++] = L;
		gc[s++] = FORWARD;
		gc[s++] = R;
	}
}

void Umbe::manageEnergy() {

	int shield = getSystemEnergy(SHIELDS)
		, missle = getSystemEnergy(MISSILES)
		, laser = getSystemEnergy(LASERS)
		, generator = getGeneratorOutput();

	if(generator < 60) {
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

	if(Lr || Rr || shield < 60 ||
	   t == 5.f || hit > shield ||
	   (missle < 80 && laser < 80) ||
	   delta(delay) < 5) {
		setSensorStatus(RRANGE, 0);
		setSensorStatus(LRANGE, 0);
	} else {
		setSensorStatus(RRANGE, 1);
		setSensorStatus(LRANGE, 1);
	}

	if(delta(delay) < .3) {
		setSensorStatus(RRADAR, 0);
		setSensorStatus(LRADAR, 0);
	} else {
		setSensorStatus(RRADAR, 1);
		setSensorStatus(LRADAR, 1);
	}

}

void Umbe::roam() {

	if(p == s - 1) p = 0;

	if(hit > getSystemEnergy(SHIELDS)) {
		t = 5.f;
		p = 0;
	}

	switch(gc[p]) {
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

	switch(gc[p]) {
		case R:
		case L:
			if(delta(time) > t) {
				p++;
				time = elapsed;
				t = .5f;
			}
			break;
		case FORWARD:
			if(delta(time) > .4f) {
				p++;
				time = elapsed;
			}
			break;
	}
}

void Umbe::onUpdate(double delta) {
	elapsed += delta;

	Lr = getSensorData(LRADAR);
	Rr = getSensorData(RRADAR);

	manageEnergy();

	Ll = getSensorData(LRANGE);
	Rl = getSensorData(RRANGE);

	if(Lr == 1 && Rr == 1) {
		if(getSystemEnergy(MISSILES) == 100) fireWeapon(MISSILE, 0);
		if(getSystemEnergy(LASERS) > 40) fireWeapon(LASER, 0);
		setMotorSpeed(-100, -100);

		delay = elapsed;

	} else if(Lr == -1 || Rr == -1) setMotorSpeed(-100, -100);
	else if(Lr == 0 && Rr == 1) {
		setMotorSpeed(40, -39);
		if(delta(delay) > 1 && delta(delay) < 10) ++delay;
	} else if(Lr == 1 && Rr == 0) {
		setMotorSpeed(-39, 40);
		if(delta(delay) > 1 && delta(delay) < 10) ++delay;
	} else if((Ll > 0 || Rl > 0) && Ll > Rl) {
		setMotorSpeed(100, 20);
		if(isBumping()) setMotorSpeed(50, -100);
	} else if((Ll > 0 || Rl > 00) && Ll > Rl) {
		setMotorSpeed(20, 100);
		if(isBumping()) setMotorSpeed(50, -100);
	} else if(delta(delay) > 1 && delta(delay) < 5) {
		setMotorSpeed(100, 100);
		if(hit > getSystemEnergy(SHIELDS)) {
			delay -= 5;
			roam();
		}
	} else if(delta(delay) < 1) {
		if(getSystemEnergy(MISSILES) == 100.f || getSystemEnergy(LASERS) > 40.f) --delay;
	} else roam();

	hit = getSystemEnergy(SHIELDS);

}

void Umbe::onStart() {

	RRADAR = addRadarGetId(45 / 2.f + 2, 45, 100);
	LRADAR = addRadarGetId(-45 / 2.f - 2, 45, 100);
	RRANGE = addRangeGetId(43, 100);
	LRANGE = addRangeGetId(317, 100);

	gl(3); //gosper curve generation
}
