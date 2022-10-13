#pragma once

enum COLOR {
	RED,
	BLUE,
	GREEN,
	YELLOW,
	RANDCOL
};

void start();

//The image background is considered as the color rgb(255, 0, 255)
void registerBot(const char *name, COLOR color, const char *img, void (*updateFn)(double), void (*initFn)(), void (*deathSignal)() = nullptr);