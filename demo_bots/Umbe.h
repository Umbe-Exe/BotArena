#pragma once
#include "..\src\competition.h"

class Umbe : public Controller {

	int RRADAR;
	int LRADAR;
	int RRANGE;
	int LRANGE;

	double elapsed = 0;

	int gc[799], s = 0, p = 0;
	double delay_attack = -10, search_time, walk_time = 0;
	int Lr = 0, Rr = 0, Ll = 0, Rl = 0; 

	bool attackSet = 0;
	bool searchSet = 0;
	bool walk_timeSet = 0;

	float target = 0;
	bool targetSet = 0;

	void rg(int st);
	void gl(int st);
	void roam();
	void manageEnergy();

	void onUpdate(double delta);

	void onStart();

	void onDeath() {}
	const char *name() {
		return "Umbe";
	}
	const char *pathToImage() {
		return "resources/images/Coke.bmp";
	}
	COLOR color() {
		return RANDCOL;
	}
};