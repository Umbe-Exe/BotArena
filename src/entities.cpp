#include "entities.h"
#include "data.h"
#include "utils.h"

Bot *bots = nullptr;
uint8_t nOfBots = 0;
Bot *currBot = nullptr;

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

		bots[i].alive = 1;

		if(data[i].initFn) {
			currBot = &bots[i];
			currBot->initFn();
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

void scatterBots() {

	for(uint8_t i = 0; i < nOfBots; ++i) {
		bots[i].x = (Rect(battleBox).bottomRight.x - Rect(battleBox).topLeft.x - 2 * botRadius) / 100 * (rand() % 101) + botRadius;
		bots[i].y = (Rect(battleBox).bottomRight.y - Rect(battleBox).topLeft.y - 2 * botRadius) / 100 * (rand() % 101) + botRadius;
		bots[i].heading = rand() % 360;
	}
}

void primeBitmaps() {

	int smallest = getSmallestSide();
	int botWidth = smallest * botRadius * 2;

	for(uint8_t i = 0; i < nOfBots; ++i) {
		
		bots[i].bitmap = al_create_bitmap(botWidth, botWidth);
		al_set_target_bitmap(bots[i].bitmap);

		drawCircle({{botRadius,botRadius},botRadius}, bots[i].color, 1.f); 
		drawFilledTriangle({
			{botRadius,0},
			{botRadius * 2,botRadius},
			{0, botRadius}}, bots[i].color);

		if(bots[i].image) {
			ALLEGRO_BITMAP *image = al_load_bitmap(bots[i].image);

			al_convert_mask_to_alpha(image, al_map_rgb(255, 0, 255));

			al_draw_scaled_bitmap(image, 0, 0, al_get_bitmap_width(image), al_get_bitmap_height(image), 0, 0, botWidth, botWidth, 0);

			al_destroy_bitmap(image);
		}
	}
}

