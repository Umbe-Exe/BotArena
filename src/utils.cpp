#include "utils.h"

static int win_h;
static int win_w;
static int smallest;

void setWidthHeight(int width, int height) {
	win_h = height;
	win_w = width;
	smallest = width < height ? width : height;
}

int getWidth() {
	return win_w;
}

int getHeight() {
	return win_h;
}

int getSmallestSide() {
	return smallest;
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
		smallest * circle.center.x,
		smallest * circle.center.y,
		smallest * circle.radius,
		color);
}

void drawCircle(Circle circle, ALLEGRO_COLOR color, float thickness) {

	al_draw_circle(
		smallest * circle.center.x,
		smallest * circle.center.y,
		smallest * circle.radius,
		color, thickness);
}

void drawFilledTriangle(Triangle triangle, ALLEGRO_COLOR color) {

	al_draw_filled_triangle(
		smallest * triangle.a.x,
		smallest * triangle.a.y,
		smallest * triangle.b.x,
		smallest * triangle.b.y,
		smallest * triangle.c.x,
		smallest * triangle.c.y,
		color);
}

float getDistance(Coord one, Coord two) {
	return sqrtf((one.x - two.x) * (one.x - two.x) + (one.y - two.y) * (one.y - two.y));
}
