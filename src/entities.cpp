#include "entities.h"
#include "data.h"
#include "utils.h"

Bot *bots = nullptr;
uint8_t nOfBots = 0;
Bot *currBot = nullptr;

ALLEGRO_BITMAP *missileBitmap = nullptr, *laserBitmap = nullptr;

Weapon **weapons = nullptr;
uint8_t nOfWeapons = 0;

void createBots(BotInitData *data, uint8_t nOfBots) {

	::nOfBots = nOfBots;

	for(uint8_t i = 0; i < nOfBots; ++i) {

		bots = (Bot *)realloc(bots, sizeof(Bot) * (i + 1));

		bots[i] = {};

		bots[i].energy = 100;
		bots[i].shield = 100;
		bots[i].missile = 100;
		bots[i].laser = 100;

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
				bots[i].color = al_map_rgb(rand() % 150,rand() % 150,rand() % 150);
		}

		bots[i].alive = 1;

		if(data[i].initFn) {
			currBot = &bots[i];
			currBot->initFn();
		}
	}
}

void destroyBots() {
	free(bots);
	nOfBots = 0;
}

void destroyWeapons() {
	al_destroy_bitmap(missileBitmap);
	al_destroy_bitmap(laserBitmap);

	for(uint8_t i = 0; i < nOfWeapons; ++i) delete weapons[i];

	free(weapons);
	nOfWeapons = 0;
}

void scatterBots() {

	uint8_t
		rows = int(Rect(battleBox).bottomRight.y / (botRadius * 2 + 0.01f)),
		cols = int(Rect(battleBox).bottomRight.x / (botRadius * 2 + 0.01f));

	bool **sector;
	sector = (bool **)_alloca(sizeof(bool *) * rows);
	for(uint8_t i = 0; i < rows; ++i) {
		sector[i] = (bool *)_alloca(sizeof(bool) * cols);

		for(int j = 0; j < cols; ++j)
			sector[i][j] = 0;
	}		

	uint8_t col, row;

	for(uint8_t i = 0; i < nOfBots; ++i) {

		do {
			col = rand() % cols;
			row = rand() % rows;
		} while(sector[row][col]);

		sector[row][col] = 1;

		bots[i].x = col * (botRadius * 2 + 0.01f) + botRadius;
		bots[i].y = row * (botRadius * 2 + 0.01f) + botRadius;

		bots[i].heading = rand() % 360;
	}
}

void primeBitmaps() {

	int smallest = getSmallestSide();

	/////////////////////////////////////

	int weaponWidth = smallest * weaponRadius * 2;

	if(missileBitmap) al_destroy_bitmap(missileBitmap);

	missileBitmap = al_create_bitmap(weaponWidth, weaponWidth);
	al_set_target_bitmap(missileBitmap);

	ALLEGRO_BITMAP *missile = al_load_bitmap("resources/images/missile.bmp");

	al_convert_mask_to_alpha(missile, al_map_rgb(255, 0, 255));

	int width = al_get_bitmap_width(missile), height = al_get_bitmap_height(missile);
	float ratio = (float)height / width;

	if(height > width)
		al_draw_scaled_bitmap(missile, 0, 0, width, height, (weaponWidth - weaponWidth / ratio) / 2, 0, weaponWidth / ratio, weaponWidth, 0);
	else
		al_draw_scaled_bitmap(missile, 0, 0, width, height, 0, (weaponWidth - weaponWidth * ratio) / 2, weaponWidth, weaponWidth * ratio, 0);

	al_destroy_bitmap(missile);

/////////////////////////////////////////

	if(laserBitmap) al_destroy_bitmap(laserBitmap);

	laserBitmap = al_create_bitmap(weaponWidth, weaponWidth);
	al_set_target_bitmap(laserBitmap);

	al_draw_line(weaponWidth / 2, 0, weaponWidth / 2, weaponWidth, al_map_rgb(255, 0, 0), smallest * 0.01f);

/////////////////////////////////////////

	int botWidth = smallest * (botRadius + 0.01) * 2;
	int small = botWidth * 0.9;

	for(uint8_t i = 0; i < nOfBots; ++i) {
		
		if(bots[i].bitmap) al_destroy_bitmap(bots[i].bitmap);

		bots[i].bitmap = al_create_bitmap(botWidth, botWidth);
		al_set_target_bitmap(bots[i].bitmap);

		drawCircle({{botRadius + 0.01,botRadius + 0.01},botRadius}, bots[i].color, 0.01f * smallest);
		drawFilledTriangle({
			{botRadius + 0.01,0.01f},
			{botRadius * 2 + 0.01,botRadius + 0.01},
			{0.01f, botRadius + 0.01}}, bots[i].color);

		if(bots[i].image) {

			ALLEGRO_BITMAP *image = al_load_bitmap(bots[i].image);

			al_convert_mask_to_alpha(image, al_map_rgb(255, 0, 255));

			int width = al_get_bitmap_width(image), height = al_get_bitmap_height(image);
			float ratio = (float)height / width;

			if(height > width)
				al_draw_scaled_bitmap(image, 0, 0, width, height, (small - small / ratio) / 2 + 0.01f + (botWidth - small) / 2, 0.01f + (botWidth - small) / 2, small / ratio, small, 0);
			else 
				al_draw_scaled_bitmap(image, 0, 0, width, height, 0.01f + (botWidth - small) / 2, (small - small * ratio) / 2 + 0.01f + (botWidth - small) / 2, small, small * ratio, 0);

			al_destroy_bitmap(image);
		}

		for(uint8_t j = 0; j < bots[i].nOfSensors; ++j) bots[i].sensor[j]->priming(smallest);
	}
}
