#include "entities.h"
#include "data.h"
#include "infobox.h"
#include <allegro5/allegro_primitives.h>

std::vector<Bot *> bots;
Bot * currBot;

ALLEGRO_BITMAP *missileBitmap = nullptr, *laserBitmap = nullptr;

std::vector<Weapon *> weapons;

void createBots(std::vector<BotInitData> &data) {

	ALLEGRO_COLOR color;

	for(auto &data : data) {

		switch(data.color) {
			case RED:
				color = {255,0,0,1};
				break;
			case BLUE:
				color = {0,0,255,1};
				break;
			case GREEN:
				color = {0,255,0,1};
				break;
			case YELLOW:
				color = {255,255,0,1};
				break;
			case RANDCOL:
			default:
				color = al_map_rgb(rand() % 150,rand() % 150,rand() % 150);
		}

		bots.push_back(new Bot(data.name, data.image, color, data.initFn, data.updateFn));

		if(data.initFn) {
			currBot = bots.back();
			currBot->initFn();
		}
	}
}

void destroyBots() {
	for(Bot *bot : bots) delete bot;

	bots.clear();
	bots.shrink_to_fit();
}

void destroyWeapons() {
	al_destroy_bitmap(missileBitmap);
	al_destroy_bitmap(laserBitmap);

	for(Weapon *i : weapons) delete i;

	weapons.clear();
	weapons.shrink_to_fit();
}

void scatterBots() {

	uint16_t botWidth = (botRadius + 0.005f) * 2 * arenaSize;

	const uint8_t size = 1.f / ((botRadius + 0.005f) * 2);

	bool sector[size][size]{};

	uint8_t col, row;

	for(Bot *bot : bots) {

		do {
			col = rand() % size;
			row = rand() % size;
		} while(sector[row][col]);

		sector[row][col] = 1;

		bot->coord = {
			col * (botRadius + 0.005f) * 2 + botRadius + 0.005f,
			row * (botRadius + 0.005f) * 2 + botRadius + 0.005f
		};

		bot->coord += battleBox.topLeft;

		bot->heading = rand() % 360;
	}
}

void primeBitmaps() {

	float weaponWidth = arenaSize * weaponRadius * 2;

	if(missileBitmap) al_destroy_bitmap(missileBitmap);

	missileBitmap = al_create_bitmap(weaponWidth, weaponWidth);
	al_set_target_bitmap(missileBitmap);

	ALLEGRO_BITMAP *missile = al_load_bitmap("resources/images/missile.bmp");

	al_convert_mask_to_alpha(missile, al_map_rgb(255, 0, 255));

	float scale;
	int width = al_get_bitmap_width(missile), height = al_get_bitmap_height(missile);

	if(height > width) scale = weaponWidth / height;
	else scale = scale = weaponWidth / width;

	if(height > width)
		al_draw_scaled_rotated_bitmap(missile, width / 2.f, height / 2.f, weaponWidth / 2.f, weaponWidth / 2.f, scale, scale, 90 * DEG_PER_RAD, 0);
	else
		al_draw_scaled_rotated_bitmap(missile, width / 2.f, height / 2.f, weaponWidth / 2.f, weaponWidth / 2.f, scale, scale, 90 * DEG_PER_RAD, 0);

	al_destroy_bitmap(missile);

/////////////////////////////////////////

	if(laserBitmap) al_destroy_bitmap(laserBitmap);

	laserBitmap = al_create_bitmap(weaponWidth, weaponWidth);
	al_set_target_bitmap(laserBitmap);

	al_draw_line(0, weaponWidth / 2, weaponWidth, weaponWidth / 2, al_map_rgb(255, 0, 0), arenaSize * 0.01f);

/////////////////////////////////////////

	float botWidth = arenaSize * botRadius * 2;
	float small = botWidth * 0.9;

	for(Bot *bot : bots) {
		
		if(bot->bitmap) al_destroy_bitmap(bot->bitmap);

		bot->bitmap = al_create_bitmap(botWidth, botWidth);
		al_set_target_bitmap(bot->bitmap);

		drawCircle({{botRadius,botRadius},botRadius - 0.005f}, bot->color, 0.01f * arenaSize);
		drawFilledTriangle({
			{botRadius,0},
			{botRadius * 2,botRadius},
			{botRadius, botRadius * 2}}, bot->color);

		if(bot->image) {

			ALLEGRO_BITMAP *image = al_load_bitmap(bot->image);

			al_convert_mask_to_alpha(image, al_map_rgb(255, 0, 255));

			int width = al_get_bitmap_width(image), height = al_get_bitmap_height(image);

			//float scale;

			if(height > width) scale = small / height;
			else scale = scale = small / width;

			if(height > width)
				al_draw_scaled_rotated_bitmap(image, width / 2.f, height / 2.f, botWidth / 2.f, botWidth / 2.f, scale, scale, 90 * DEG_PER_RAD, 0);
			else 
				al_draw_scaled_rotated_bitmap(image, width / 2.f, height / 2.f, botWidth / 2.f, botWidth / 2.f, scale, scale, 90 * DEG_PER_RAD, 0);

			al_destroy_bitmap(image);
		}

		currBot = bot;
		for(Sensor *sensor : bot->sensors) sensor->priming();
	}

/////////////////////////////////////////

	primeInfoboxBitmap();
}
