#pragma once
#include "Arena.h"
#include <allegro5/allegro.h>
#include <allegro5/allegro_primitives.h>

struct BotInitData {
	const char *name;
	COLOR color;
	const char *image;
	void (*updateFn)(float);
	void (*initFn)();
};

void createBots(BotInitData *data, uint8_t nOfBots);

void destroyBots();

void destroyWeapons();

void scatterBots();

void primeBitmaps();