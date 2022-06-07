#include "utils.h"

static int win_h;
static int win_w;

void setWidthHeight(int width, int height) {
	win_h = height;
	win_w = width;
}

float toWin_hCoord(float y) {
	return y * win_h;
}

float toWin_wCoord(float x) {
	return x * win_w;
}

void drawFilledRect(Rect rect, ALLEGRO_COLOR color) {

	al_draw_filled_rectangle(
		toWin_wCoord(rect.topLeft.x),
		toWin_hCoord(rect.topLeft.y),
		toWin_wCoord(rect.bottomRight.x),
		toWin_hCoord(rect.bottomRight.y),
		color);
}

void drawRect(Rect rect, ALLEGRO_COLOR color, float thickness) {

	al_draw_rectangle(
		toWin_wCoord(rect.topLeft.x),
		toWin_hCoord(rect.topLeft.y),
		toWin_wCoord(rect.bottomRight.x),
		toWin_hCoord(rect.bottomRight.y),
		color, thickness);
}

void drawFilledCircle(Circle circle, ALLEGRO_COLOR color) {

	al_draw_filled_circle(
		toWin_wCoord(circle.center.x),
		toWin_hCoord(circle.center.y),
		circle.radius,
		color);
}

void drawCircle(Circle circle, ALLEGRO_COLOR color, float thickness) {

	al_draw_circle(
		toWin_wCoord(circle.center.x),
		toWin_hCoord(circle.center.y),
		circle.radius,
		color, thickness);
}
