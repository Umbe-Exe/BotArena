#include "entities.h"
#include "data.h"
#include "utils.h"
#include <vector>

std::vector<Bot> bots;
std::vector<Bot>::iterator currBot;

ALLEGRO_BITMAP *missileBitmap = nullptr, *laserBitmap = nullptr;

std::vector<Weapon *> weapons;

void createBots(std::vector<BotInitData> &data) {

	for(auto& data : data) {

		bots.push_back({});

		bots.back().energy = 100;
		bots.back().shield = 100;
		bots.back().missile = 100;
		bots.back().laser = 100;

		bots.back().name = data.name;
		bots.back().image = data.image;
		bots.back().updateFn = data.updateFn;
		bots.back().initFn = data.initFn;

		switch(data.color) {
			case RED:
				bots.back().color = {255,0,0,1};
				break;
			case BLUE:
				bots.back().color = {0,0,255,1};
				break;
			case GREEN:
				bots.back().color = {0,255,0,1};
				break;
			case YELLOW:
				bots.back().color = {255,255,0,1};
				break;
			case RANDCOL:
			default:
				bots.back().color = al_map_rgb(rand() % 150,rand() % 150,rand() % 150);
		}

		bots.back().alive = 1;

		if(data.initFn) {
			currBot = bots.end() - 1;
			currBot->initFn();
		}
	}
}

void destroyBots() {
	bots.clear();
	bots.shrink_to_fit();
}

void destroyWeapons() {
	al_destroy_bitmap(missileBitmap);
	al_destroy_bitmap(laserBitmap);

	for(auto &i : weapons) delete i;
}

void scatterBots() {

	uint16_t width = getWidth(), height = getHeight(), smallest = getSmallestSide();
	uint16_t botWidth = (botRadius + 0.005f) * 2 * smallest;

	uint8_t
		rows = int(Rect(battleBox).bottomRight.y * height / botWidth),
		cols = int(Rect(battleBox).bottomRight.x * width / botWidth);

	bool **sector;
	sector = (bool **)_alloca(sizeof(bool *) * rows);
	for(uint8_t i = 0; i < rows; ++i) {
		sector[i] = (bool *)_alloca(sizeof(bool) * cols);

		for(int j = 0; j < cols; ++j)
			sector[i][j] = 0;
	}

	uint8_t col, row;

	for(auto &bot : bots) {

		do {
			col = rand() % cols;
			row = rand() % rows;
		} while(sector[row][col]);

		sector[row][col] = 1;

		bot.coord = {
			col * (botRadius + 0.005f) * 2 + botRadius + 0.005f,
			row * (botRadius + 0.005f) * 2 + botRadius + 0.005f
		};

		bot.heading = rand() % 360;
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

	int botWidth = smallest * (botRadius) * 2;
	int small = botWidth * 0.9;

	for(auto &bot : bots) {
		
		if(bot.bitmap) al_destroy_bitmap(bot.bitmap);

		bot.bitmap = al_create_bitmap(botWidth, botWidth);
		al_set_target_bitmap(bot.bitmap);

		drawCircle({{botRadius,botRadius},botRadius - 0.005f}, bot.color, 0.01f * smallest);
		drawFilledTriangle({
			{botRadius,0},
			{botRadius * 2,botRadius },
			{0, botRadius}}, bot.color);

		if(bot.image) {

			ALLEGRO_BITMAP *image = al_load_bitmap(bot.image);

			al_convert_mask_to_alpha(image, al_map_rgb(255, 0, 255));

			int width = al_get_bitmap_width(image), height = al_get_bitmap_height(image);
			float ratio = (float)height / width;

			if(height > width)
				al_draw_scaled_bitmap(image, 0, 0, width, height, (small - small / ratio) / 2 + (botWidth - small) / 2, (botWidth - small) / 2, small / ratio, small, 0);
			else 
				al_draw_scaled_bitmap(image, 0, 0, width, height, (botWidth - small) / 2, (small - small * ratio) / 2 + (botWidth - small) / 2, small, small * ratio, 0);

			al_destroy_bitmap(image);
		}

		for(auto &sensor : bot.sensors)
			sensor->priming();
	}
}
