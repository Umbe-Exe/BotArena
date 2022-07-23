#pragma once
#include "Arena.h"
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