#pragma once
#include "..\src\competition.h"

class Umbe : public Controller {

	int RRADAR;
	int LRADAR;
	int RRANGE;
	int LRANGE;

	double elapsed = 0;

	int gc[799], s = 0, p = 0;
	double time = 0; 
	double delay = -10;
	int Lr = 0, Rr = 0, Ll = 0, Rl = 0;

	int hit = 0;
	double t = .5;

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