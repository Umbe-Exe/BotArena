#include "..\src\competition.h"
#include "Umbe.h"
#include <cmath>

#define R 0
#define L 1
#define FORWARD 3

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

	if(Lr || Rr || shield < 60 || searchSet ||
	   (missle < 80 && laser < 80) ||
	   attackSet) {
		setSensorStatus(RRANGE, 0);
		setSensorStatus(LRANGE, 0);
	} else {
		setSensorStatus(RRANGE, 1);
		setSensorStatus(LRANGE, 1);
	}

	if(attackSet && elapsed - delay_attack < .3f) {
		setSensorStatus(RRADAR, 0);
		setSensorStatus(LRADAR, 0);
	} else {
		setSensorStatus(RRADAR, 1);
		setSensorStatus(LRADAR, 1);
	}

}

void Umbe::roam() {

	if(p == s - 1) p = 0;

	switch(gc[p]) {
		case R:
			if(!targetSet) {
				target = getGPSdata().heading + 60;
				if(target > 360) target -= 360;
				targetSet = 1;
				setMotorSpeed(100, -100);
			}
			break;
		case L:
			if(!targetSet) {
				target = getGPSdata().heading - 60;
				if(target < 0) target += 360;
				targetSet = 1;
				setMotorSpeed(-100, 100);
			}
			break;
		case FORWARD:
			if(!walk_timeSet) {
				walk_time = elapsed + .6f;
				walk_timeSet = 1;
				setMotorSpeed(100, 100);
			}
			if(!targetSet&&walk_timeSet&&walk_time - elapsed < 0) {
				p++;
				walk_timeSet = 0;
			}
			break;
	}

	if(targetSet&&abs(getGPSdata().heading - target) < 5.f) {
		p++;
		targetSet = 0;
	}
}

void Umbe::onUpdate(double delta) {
	elapsed += delta;

	Lr = getSensorData(LRADAR);
	Rr = getSensorData(RRADAR);

	manageEnergy();

	Ll = getSensorData(LRANGE);
	Rl = getSensorData(RRANGE);


	if(isBumping() & MissileHit || isBumping() & LaserHit || isBumping() & BotCollision) search_time = elapsed + 1.f;

	if(Lr == 1 && Rr == 1) {
		if(getSystemEnergy(MISSILES) == 100) fireWeapon(MISSILE, 0);
		if(getSystemEnergy(LASERS) > 40) fireWeapon(LASER, 0);
		setMotorSpeed(-100, -100);

		delay_attack = elapsed + 5.f;

	} else if(Lr == -1 || Rr == -1) setMotorSpeed(-100, -100);
	else if(Lr == 0 && Rr == 1) {
		setMotorSpeed(100, -99);
		if(delay_attack - elapsed < 1 && delay_attack - elapsed > 5) ++delay_attack;
	} else if(Lr == 1 && Rr == 0) {
		setMotorSpeed(-99, 100);
		if(delay_attack - elapsed < 1 && delay_attack - elapsed > 5) ++delay_attack;
	} else if(search_time > elapsed) setMotorSpeed(-100, 100);
	else if(delay_attack > elapsed) {
		if(delay_attack - elapsed < 1 && delay_attack - elapsed > 5) {
			setMotorSpeed(100, 100);
			if(isBumping() & MissileHit || isBumping() & LaserHit || isBumping() & BotCollision) {
				delay_attack -= 5;
				search_time = elapsed + 1.f;
			}
		} else if(delay_attack - elapsed > 4) {
			if(getSystemEnergy(MISSILES) == 100.f || getSystemEnergy(LASERS) > 40.f) setMotorSpeed(100, 100);
		}
	} else if(Ll > Rl) {
		
		if(!target) {
			setMotorSpeed(100, -100);
			target = getGPSdata().heading + 10;
			if(target > 360) target -= 360;
			targetSet = 1;
		}
	} else if(Ll < Rl) {
		
		if(!target) {
			setMotorSpeed(-100, 100);
		target = getGPSdata().heading - 10;
		if(target < 0) target += 360;
		targetSet = 1;
		}
	} else roam();

}

void Umbe::onStart() {

	RRADAR = addRadarGetId(45 / 2.f + 2, 45, 100);
	LRADAR = addRadarGetId(-45 / 2.f - 2, 45, 100);
	RRANGE = addRangeGetId(43, 100);
	LRANGE = addRangeGetId(317, 100);

	gl(3); //gosper curve generation
}
