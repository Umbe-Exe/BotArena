#pragma once

enum COLOR {
	RED,
	BLUE,
	GREEN,
	YELLOW,
	RANDCOL
};

void start();

void registerBot(const char *name, COLOR color, const char *img, void *updateFn, void *initFn);