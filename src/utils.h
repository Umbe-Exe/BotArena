#pragma once
#include <allegro5/allegro.h>
#include <allegro5/allegro_primitives.h>
#include <math.h>

#define RAD_PER_DEG 1.7453292519943295E-2

struct Coord {
	float x, y;

	Coord operator+(Coord right) const {
		return {x + right.x, y + right.y};
	}
	Coord operator-(Coord right) const {
		return {x - right.x, y - right.y};
	}
	Coord operator+=(Coord right) {
		x += right.x;
		y += right.y;
		return *this;
	}
	Coord operator-=(Coord right) {
		x -= right.x;
		y -= right.y;
		return *this;
	}
	Coord operator+(float right) const {
		return {x + right, y + right};
	}
	Coord operator-(float right) const {
		return {x - right, y - right};
	}
	Coord operator+=(float right) {
		x += right;
		y += right;
		return *this;
	}
	Coord operator-=(float right) {
		x -= right;
		y -= right;
		return *this;
	}
	Coord operator*(float right) const {
		return {x * right, y * right};
	}
	Coord operator*=(float right) {
		x *= right;
		y *= right;
		return *this;
	}
	float dot(Coord right) const {
		return x * right.x + y * right.y;
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

void makeBattleBox();

void drawFilledRect(Rect rect, ALLEGRO_COLOR color);
void drawRect(Rect rect, ALLEGRO_COLOR color, float thickness);
void drawFilledCircle(Circle circle, ALLEGRO_COLOR color);
void drawCircle(Circle circle, ALLEGRO_COLOR color, float thickness);
void drawFilledTriangle(Triangle triangle, ALLEGRO_COLOR color);

float getDistance(Coord one, Coord two);