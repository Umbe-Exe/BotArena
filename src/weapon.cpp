#include "weapon.h"
#include "data.h"

void Missile::draw() {

	al_draw_rotated_bitmap(missileBitmap,
			       al_get_bitmap_width(missileBitmap) / 2,
			       al_get_bitmap_height(missileBitmap) / 2,
			       coord.x * getSmallestSide(), coord.y * getSmallestSide(), heading, 0);
}

void Missile::update(float delta) {}

void Laser::draw() {

	al_draw_rotated_bitmap(laserBitmap,
			       al_get_bitmap_width(laserBitmap) / 2,
			       al_get_bitmap_height(laserBitmap) / 2,
			       coord.x * getSmallestSide(), coord.y * getSmallestSide(), heading, 0);
}

void Laser::update(float delta) {}
