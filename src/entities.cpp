#include "entities.h"
#include "data.h"

Bot *bots = nullptr;
uint8_t nOfBots = 0;

ALLEGRO_BITMAP *missileBitmap = nullptr, *laserBitmap = nullptr;

Weapon *weapons = nullptr;
uint8_t nOfWeapons = 0;

void createBots(BotInitData *data, uint8_t nOfBots) {

	::nOfBots = nOfBots;

	for(uint8_t i = 0; i < nOfBots; ++i) {

		bots = (Bot *)realloc(bots, sizeof(Bot) * (i + 1));
		if(!bots) abort();

		bots[i] = {};

		bots[i].name = data[i].name;
		bots[i].image = data[i].image;
		bots[i].updateFn = data[i].updateFn;
		bots[i].initFn = data[i].initFn;

		switch(data[i].color) {
			case RED:
				bots[i].color = {255,0,0,1};
				break;
			case BLUE:
				bots[i].color = {0,0,255,1};
				break;
			case GREEN:
				bots[i].color = {0,255,0,1};
				break;
			case YELLOW:
				bots[i].color = {255,255,0,1};
				break;
			case RANDCOL:
			default:
				bots[i].color = {float(rand() % 200 + 56),float(rand() % 200 + 56),float(rand() % 200 + 56),1};
		}
	}
}

void destroyBots() {

	for(uint8_t i = 0; i < nOfBots; ++i) {

		for(uint8_t j = 0; j < bots[i].nOfSensors; ++j) al_destroy_bitmap(bots[i].sensor[j].bitmap);

		al_destroy_bitmap(bots[i].bitmap);
		free(bots[i].sensor);
	}
	free(bots);
}

void destroyWeapons() {
	al_destroy_bitmap(missileBitmap);
	al_destroy_bitmap(laserBitmap);
	free(weapons);
}

