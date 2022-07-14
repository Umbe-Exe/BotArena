#include "utils.h"
#include "data.h"

int win_w, win_h;
float arenaSize;

Rect battleBox{};

void transposeEntities(Coord vector) {
	for(Bot *bot : bots) bot->coord += vector;
	for(Weapon *weapon : weapons) weapon->coord += vector;
}

void makeBattleBox() {
	win_w = al_get_display_width(window);
	win_h = al_get_display_height(window);

	if(Rect(infoBox).topLeft.x * win_w < win_h) arenaSize = Rect(infoBox).topLeft.x * win_w;
	else arenaSize = win_h;

	Coord location = {Rect(infoBox).topLeft.x / 2.f - arenaSize / win_w / 2.f,  0.5f - arenaSize / win_h / 2.f};

	transposeEntities(location - battleBox.topLeft);

	battleBox = {location,location + Coord{arenaSize / win_w,arenaSize / win_h}};
}

void drawFilledRect(Rect rect, ALLEGRO_COLOR color) {

	al_draw_filled_rectangle(
		win_w * rect.topLeft.x,
		win_h * rect.topLeft.y,
		win_w * rect.bottomRight.x,
		win_h * rect.bottomRight.y,
		color);
}

void drawRect(Rect rect, ALLEGRO_COLOR color, float thickness) {

	al_draw_rectangle(
		win_w * rect.topLeft.x,
		win_h * rect.topLeft.y,
		win_w * rect.bottomRight.x,
		win_h * rect.bottomRight.y,
		color, thickness);
}

void drawFilledCircle(Circle circle, ALLEGRO_COLOR color) {

	al_draw_filled_circle(
		arenaSize * circle.center.x,
		arenaSize * circle.center.y,
		arenaSize * circle.radius,
		color);
}

void drawCircle(Circle circle, ALLEGRO_COLOR color, float thickness) {

	al_draw_circle(
		arenaSize * circle.center.x,
		arenaSize * circle.center.y,
		arenaSize * circle.radius,
		color, thickness);
}

void drawFilledTriangle(Triangle triangle, ALLEGRO_COLOR color) {

	al_draw_filled_triangle(
		arenaSize * triangle.a.x,
		arenaSize * triangle.a.y,
		arenaSize * triangle.b.x,
		arenaSize * triangle.b.y,
		arenaSize * triangle.c.x,
		arenaSize * triangle.c.y,
		color);
}

float getDistance(Coord one, Coord two) {
	return sqrtf((one.x - two.x) * (one.x - two.x) + (one.y - two.y) * (one.y - two.y));
}
