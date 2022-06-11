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
		toWin_wCoord(circle.center.x),
		toWin_hCoord(circle.center.y),
		smallest * circle.radius,
		color);
}

void drawCircle(Circle circle, ALLEGRO_COLOR color, float thickness) {

	al_draw_circle(
		toWin_wCoord(circle.center.x),
		toWin_hCoord(circle.center.y),
		smallest * circle.radius,
		color, thickness);
}

void drawFilledTriangle(Triangle triangle, ALLEGRO_COLOR color) {

	al_draw_filled_triangle(
		toWin_wCoord(triangle.a.x),
		toWin_hCoord(triangle.a.y),
		toWin_wCoord(triangle.b.x),
		toWin_hCoord(triangle.b.y),
		toWin_wCoord(triangle.c.x),
		toWin_hCoord(triangle.c.y),
		color);
}
