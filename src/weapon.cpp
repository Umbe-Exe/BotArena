#include "weapon.h"
#include "data.h"

void Missile::draw() {

	al_draw_rotated_bitmap(missileBitmap, 
						   al_get_bitmap_height(missileBitmap)/2,
						   al_get_bitmap_width(missileBitmap) / 2,
						   )

}

void Missile::update(float delta) {}

void Laser::draw() {}

void Laser::update(float delta) {}
