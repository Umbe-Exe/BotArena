#include "infobox.h"
#include "data.h"
#include <allegro5/allegro_primitives.h>
#include <allegro5/allegro_ttf.h>

ALLEGRO_BITMAP *bitmap = 0;
ALLEGRO_FONT *nameFont = 0, *sysFont = 0;

void primeInfoboxBitmap() {

	uint8_t q = (bots.size() > 7 ? 7 : bots.size());

	if(nameFont) al_destroy_font(nameFont);
	nameFont = al_load_ttf_font("resources/font/typed.ttf", 0.05f * arenaSize / (q / 4.f), 0);

	float nameFontHeight = (float)al_get_font_line_height(nameFont);

	if(sysFont) al_destroy_font(sysFont);
	sysFont = al_load_ttf_font("resources/font/typed.ttf", (win_h - nameFontHeight * q) / q / 5.f, 0);

	if(bitmap) al_destroy_bitmap(bitmap);
	bitmap = al_create_bitmap(
		(Rect(infoBox).bottomRight.x - Rect(infoBox).topLeft.x) * win_w,
		Rect(infoBox).bottomRight.y * win_h);

	al_set_target_bitmap(bitmap);
	al_clear_to_color(al_map_rgb(200, 200, 200));

	uint16_t width = al_get_bitmap_width(bitmap);

	float sysFontHeight = (float)al_get_font_line_height(sysFont);
	float sysFontWidth = (float)al_get_text_width(sysFont, "Generator");

	bool labels = 1;

	if(sysFontWidth > width / 2.f) {
		(win_h - nameFontHeight * q) / q / 4.f;
		sysFontWidth = 0;
		labels = 0;
	}

	for(uint8_t i = 0; i < q; ++i) {
		al_draw_text(nameFont, bots[i]->color, width / 2, win_h / q * i, ALLEGRO_ALIGN_CENTER, (bots[i]->name ? bots[i]->name : "UNNAMED"));

		if(labels) {
			al_draw_text(sysFont, bots[i]->color, 0, win_h / q * i + nameFontHeight, 0, "Generator");
			al_draw_text(sysFont, bots[i]->color, 0, win_h / q * i + sysFontHeight + nameFontHeight, 0, "Shield");
			al_draw_text(sysFont, bots[i]->color, 0, win_h / q * i + sysFontHeight * 2.f + nameFontHeight, 0, "Missile");
			al_draw_text(sysFont, bots[i]->color, 0, win_h / q * i + sysFontHeight * 3.f + nameFontHeight, 0, "Laser");
		}

		al_draw_rectangle(
			sysFontWidth + sysFontWidth / 9.f,
			win_h / q * i + nameFontHeight,
			width,
			win_h / q * i + sysFontHeight + nameFontHeight,
			bots[i]->color, 0.005f * arenaSize);

		al_draw_rectangle(
			sysFontWidth + sysFontWidth / 9.f,
			win_h / q * i + sysFontHeight + nameFontHeight,
			width,
			win_h / q * i + sysFontHeight * 2.f + nameFontHeight,
			bots[i]->color, 0.005f * arenaSize);

		al_draw_rectangle(
			sysFontWidth + sysFontWidth / 9.f,
			win_h / q * i + sysFontHeight * 2.f + nameFontHeight,
			width,
			win_h / q * i + sysFontHeight * 3.f + nameFontHeight,
			bots[i]->color, 0.005f * arenaSize);

		al_draw_rectangle(
			sysFontWidth + sysFontWidth / 9.f,
			win_h / q * i + sysFontHeight * 3.f + nameFontHeight,
			width,
			win_h / q * i + sysFontHeight * 4.f + nameFontHeight,
			bots[i]->color, 0.005f * arenaSize);
	}
}

void drawInfobox() {

	al_draw_bitmap(bitmap, Rect(infoBox).topLeft.x * win_w, 0, 0);

}

void destroyInfobox() {
	al_destroy_bitmap(bitmap);
	al_destroy_font(nameFont);
	al_destroy_font(sysFont);
}
