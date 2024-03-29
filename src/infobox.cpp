#include "arena_impl.h"
#include "common.h"

void Arena_Impl::primeInfoboxBitmap() {

	nOfBots = (uint8_t)bots.size();

	if(nameFont) al_destroy_font(nameFont);
	nameFont = al_load_ttf_font("resources/font/typed.ttf", 0.05f * arenaSize, 0);

	if(sysFont) al_destroy_font(sysFont);
	sysFont = al_load_ttf_font("resources/font/typed.ttf", 0.02f * arenaSize, 0);

	nameFontHeight = al_get_font_line_height(nameFont);
	sysFontHeight = al_get_font_line_height(sysFont);

	if(infoboxBitmap) al_destroy_bitmap(infoboxBitmap);
	infoboxBitmap = al_create_bitmap(
		(infoBox.bottomRight.x - infoBox.topLeft.x) * win_w,
		(nameFontHeight + sysFontHeight * 4) * nOfBots + (nameFontHeight + sysFontHeight) * (int)terminatedBots.size());

	al_set_target_bitmap(infoboxBitmap);
	al_clear_to_color(al_map_rgb(200, 200, 200));

	bitmapWidth = al_get_bitmap_width(infoboxBitmap);
	bitmapHeight = al_get_bitmap_height(infoboxBitmap);

	bitmapX = win_w - bitmapWidth;

	sysFontWidth = al_get_text_width(sysFont, "Generator");

	for(uint8_t i = 0; i < nOfBots; ++i) {
		al_draw_text(nameFont, bots[i].color, bitmapWidth / 2.f, (nameFontHeight + sysFontHeight * 4) * i, ALLEGRO_ALIGN_CENTER, (bots[i].name ? bots[i].name : "UNNAMED"));

		al_draw_text(sysFont, bots[i].color, 0, nameFontHeight + (nameFontHeight + sysFontHeight * 4) * i, 0, "Generator");
		al_draw_text(sysFont, bots[i].color, 0, nameFontHeight + (nameFontHeight + sysFontHeight * 4) * i + sysFontHeight, 0, "Shield");
		al_draw_text(sysFont, bots[i].color, 0, nameFontHeight + (nameFontHeight + sysFontHeight * 4) * i + sysFontHeight * 2.f, 0, "Missile");
		al_draw_text(sysFont, bots[i].color, 0, nameFontHeight + (nameFontHeight + sysFontHeight * 4) * i + sysFontHeight * 3.f, 0, "Laser");

		al_draw_rectangle(
			sysFontWidth + sysFontWidth / 9.f,
			nameFontHeight + (nameFontHeight + sysFontHeight * 4) * i,
			bitmapWidth * 0.9f,
			nameFontHeight + (nameFontHeight + sysFontHeight * 4) * i + sysFontHeight,
			bots[i].color, 0.005f * arenaSize);

		al_draw_rectangle(
			sysFontWidth + sysFontWidth / 9.f,
			nameFontHeight + (nameFontHeight + sysFontHeight * 4) * i + sysFontHeight,
			bitmapWidth * 0.9f,
			nameFontHeight + (nameFontHeight + sysFontHeight * 4) * i + sysFontHeight * 2.f,
			bots[i].color, 0.005f * arenaSize);

		al_draw_rectangle(
			sysFontWidth + sysFontWidth / 9.f,
			nameFontHeight + (nameFontHeight + sysFontHeight * 4) * i + sysFontHeight * 2.f,
			bitmapWidth * 0.9f,
			nameFontHeight + (nameFontHeight + sysFontHeight * 4) * i + sysFontHeight * 3.f,
			bots[i].color, 0.005f * arenaSize);

		al_draw_rectangle(
			sysFontWidth + sysFontWidth / 9.f,
			nameFontHeight + (nameFontHeight + sysFontHeight * 4) * i + sysFontHeight * 3.f,
			bitmapWidth * 0.9f,
			nameFontHeight + (nameFontHeight + sysFontHeight * 4) * i + sysFontHeight * 4.f,
			bots[i].color, 0.005f * arenaSize);
	}

	ALLEGRO_COLOR color;
	for(uint8_t i = 0; i < terminatedBots.size(); ++i) {

		color = al_map_rgb(rand() % 150, rand() % 150, rand() % 150);
		al_draw_text(nameFont, color, bitmapWidth / 2.f, (nameFontHeight + sysFontHeight * 4) * nOfBots + (nameFontHeight + sysFontHeight) * i, ALLEGRO_ALIGN_CENTER, (terminatedBots[i] ? terminatedBots[i] : "UNNAMED"));
		al_draw_text(sysFont, color, bitmapWidth / 2.f, (nameFontHeight + sysFontHeight * 4) * nOfBots + (nameFontHeight + sysFontHeight) * i + nameFontHeight, ALLEGRO_ALIGN_CENTER, "terminated");
	}
}

void Arena_Impl::infoBoxScroll(float to) {
	if(bitmapHeight < win_h) scrollPos = 0;
	else {
		scrollPos -= to * nameFontHeight;
		if(scrollPos < 0) scrollPos = 0;
		else if(scrollPos > bitmapHeight - win_h) scrollPos = bitmapHeight - win_h;
	}
}

void Arena_Impl::drawInfobox() {
	al_draw_bitmap_region(infoboxBitmap, 0, scrollPos, bitmapWidth, win_h, win_w - bitmapWidth, 0, 0);

	uint8_t start = 0, end = 0;

	if(bitmapHeight < win_h) {
		start = 0;
		end = nOfBots;
	} else if(bitmapHeight - scrollPos - (nameFontHeight + sysFontHeight) * terminatedBots.size() < win_h) {
		if(bitmapHeight - scrollPos - (nameFontHeight + sysFontHeight) * terminatedBots.size() > 0) {
			start = scrollPos / (nameFontHeight + sysFontHeight * 4);
			end = nOfBots;
		}
	} else {
		start = scrollPos / (nameFontHeight + sysFontHeight * 4);
		end = (scrollPos + win_h) / (nameFontHeight + sysFontHeight * 4) + 1;
		if(end > nOfBots) --end;
	}

	for(uint8_t i = start; i < end; ++i) {

		al_draw_filled_rectangle(
			bitmapX + sysFontWidth + sysFontWidth / 9.f,
			nameFontHeight + (nameFontHeight + sysFontHeight * 4) * i - scrollPos,
			bitmapX + sysFontWidth + sysFontWidth / 9.f + (bitmapWidth * 0.9f - (sysFontWidth + sysFontWidth / 9.f)) / maxGeneratorStructure * bots[i].generator,
			nameFontHeight + (nameFontHeight + sysFontHeight * 4) * i + sysFontHeight - scrollPos,
			bots[i].color);

		al_draw_filled_rectangle(
			bitmapX + sysFontWidth + sysFontWidth / 9.f,
			nameFontHeight + (nameFontHeight + sysFontHeight * 4) * i + sysFontHeight - scrollPos,
			bitmapX + sysFontWidth + sysFontWidth / 9.f + (bitmapWidth * 0.9f - (sysFontWidth + sysFontWidth / 9.f)) / maxShield * bots[i].shield,
			nameFontHeight + (nameFontHeight + sysFontHeight * 4) * i + sysFontHeight * 2.f - scrollPos,
			bots[i].color);

		al_draw_filled_rectangle(
			bitmapX + sysFontWidth + sysFontWidth / 9.f,
			nameFontHeight + (nameFontHeight + sysFontHeight * 4) * i + sysFontHeight * 2.f - scrollPos,
			bitmapX + sysFontWidth + sysFontWidth / 9.f + (bitmapWidth * 0.9f - (sysFontWidth + sysFontWidth / 9.f)) / maxMissile * bots[i].missile,
			nameFontHeight + (nameFontHeight + sysFontHeight * 4) * i + sysFontHeight * 3.f - scrollPos,
			bots[i].color);

		al_draw_filled_rectangle(
			bitmapX + sysFontWidth + sysFontWidth / 9.f,
			nameFontHeight + (nameFontHeight + sysFontHeight * 4) * i + sysFontHeight * 3.f - scrollPos,
			bitmapX + sysFontWidth + sysFontWidth / 9.f + (bitmapWidth * 0.9f - (sysFontWidth + sysFontWidth / 9.f)) / maxLaser * bots[i].laser,
			nameFontHeight + (nameFontHeight + sysFontHeight * 4) * i + sysFontHeight * 4.f - scrollPos,
			bots[i].color);
	}
}

void Arena_Impl::destroyInfobox() {
	if(infoboxBitmap) al_destroy_bitmap(infoboxBitmap);
	if(nameFont) al_destroy_font(nameFont);
	if(sysFont) al_destroy_font(sysFont);
}