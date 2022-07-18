#pragma once
#include "Arena.h"
#include <allegro5/allegro.h>
#include <allegro5/allegro_primitives.h>
#include <allegro5/allegro_image.h>
#include <vector>

struct BotInitData {
	const char *name;
	COLOR color;
	const char *image;
	void (*updateFn)(double);
	void (*initFn)();
};

void createBots(std::vector<BotInitData> &data);

void destroyBots();

void destroyWeapons();

void scatterBots();

void primeBitmaps();