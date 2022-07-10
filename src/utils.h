#pragma once
#include <allegro5/allegro.h>
#include <allegro5/allegro_primitives.h>
#include <math.h>

#define RAD_PER_DEG 1.7453292519943295E-2

struct Coord {
	float x, y;

	Coord operator+(Coord right) const {
		return {this->x + right.x, this->y + right.y};
	}
	Coord operator-(Coord right) const {
		return {this->x - right.x, this->y - right.y};
	}
	Coord operator+=(Coord right) {
		this->x += right.x;
		this->y += right.y;
		return *this;
	}
	Coord operator-=(Coord right) {
		this->x -= right.x;
		this->y -= right.y;
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

struct Triangle {
	Coord a, b, c;
};

int getWidth();
int getHeight();
int getSmallestSide();

float toWin_hCoord(float y);
float toWin_wCoord(float x);

void setWidthHeight(int width, int height);

void drawFilledRect(Rect rect, ALLEGRO_COLOR color);
void drawRect(Rect rect, ALLEGRO_COLOR color, float thickness);
void drawFilledCircle(Circle circle, ALLEGRO_COLOR color);
void drawCircle(Circle circle, ALLEGRO_COLOR color, float thickness);
void drawFilledTriangle(Triangle triangle, ALLEGRO_COLOR color);