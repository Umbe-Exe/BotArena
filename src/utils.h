#pragma once
#include "common.h"

#define DEG_PER_RAD 1.7453292519943295E-2
#define RAD_PER_DEG 5.7295779513082320E1 
#define PI 3.14159265359

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

inline float getDistance(Coord one, Coord two) {
	return sqrtf((one.x - two.x) * (one.x - two.x) + (one.y - two.y) * (one.y - two.y));
}

inline float Signed2DTriArea(Coord a, Coord b, Coord c) {
	return (a.x - c.x) * (b.y - c.y) - (a.y - c.y) * (b.x - c.x);
}

inline void segment_segment_intersection(Coord a, Coord b, Coord c, Coord d, std::set<float> &t) {
	float a1 = Signed2DTriArea(a, b, d);
	float a2 = Signed2DTriArea(a, b, c);

	if(a1 * a2 < 0.f) {
		float a3 = Signed2DTriArea(c, d, a);
		float a4 = a3 + a2 - a1;

		if(a3 * a4 < 0.f) t.insert(a3 / (a3 - a4));
	}
}

inline float segmentCircleIntersection(float radius, Coord from, Coord d, Coord to) {

	Coord m;
	float b, c, discr;

	m = from - to;
	b = m.dot(d);
	c = m.dot(m) - radius * radius;

	if((c > 0.f && b > 0.f) ||
	   (discr = b * b - c) < 0.f) return 0;

	return (-b - sqrtf(discr));
}

inline bool circleCircleSectorIntersection(Coord circle, Coord start, Coord p, Coord end) {

	p -= circle;

	float S = atan2f(start.y, start.x);
	float A = atan2f(p.y, p.x);
	float E = atan2f(end.y, end.x);

	if(S < E) return S < A &&A < E;
	else if(S > E && (A > S || A < E)) return 1;
	return 0;
}

inline void drawRect(float scale, Rect rect, ALLEGRO_COLOR color, float thickness) {

	al_draw_rectangle(
		scale * rect.topLeft.x,
		scale * rect.topLeft.y,
		scale * rect.bottomRight.x,
		scale * rect.bottomRight.y,
		color, thickness);
}

inline void drawCircle(float scale, Circle circle, ALLEGRO_COLOR color, float thickness) {

	al_draw_circle(
		scale * circle.center.x,
		scale * circle.center.y,
		scale * circle.radius,
		color, thickness);
}

inline void drawFilledTriangle(float scale, Triangle triangle, ALLEGRO_COLOR color) {

	al_draw_filled_triangle(
		scale * triangle.a.x,
		scale * triangle.a.y,
		scale * triangle.b.x,
		scale * triangle.b.y,
		scale * triangle.c.x,
		scale * triangle.c.y,
		color);
}
