#pragma once
#include <allegro5/allegro.h>
#include <allegro5/allegro_primitives.h>

struct Coord {
	float x, y;

	Coord operator+(Coord right) const {
		return {this->x + right.x, this->x + right.y};
	}
	Coord operator-(Coord right) const {
		return {this->x - right.x, this->x - right.y};
	}
	Coord operator+=(Coord right) {
		this->x += right.x;
		this->x += right.y;
		return *this;
	}
	Coord operator-=(Coord right) {
		this->x -= right.x;
		this->x -= right.y;
		return *this;
	}
};

struct Rect {
	Coord topLeft, bottomRight;
};

struct Circle {
	Coord center;
	float radius;
};

void setWidthHeight(int width, int height);

void drawFilledRect(Rect rect, ALLEGRO_COLOR color);
void drawRect(Rect rect, ALLEGRO_COLOR color, float thickness);
void drawFilledCircle(Circle circle, ALLEGRO_COLOR color);
void drawCircle(Circle circle, ALLEGRO_COLOR color, float thickness);