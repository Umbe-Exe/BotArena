#include "update.h"
#include "utils.h"
#include <allegro5/allegro.h>
#include <allegro5/allegro_primitives.h>

#include "data.h"

void logic(double delta) {



}

void draw() {

	al_clear_to_color(al_map_rgb(200,200,200));

	for(uint8_t i = 0; i < nOfBots; ++i) bots[i].draw();
	for(uint8_t i = 0; i < nOfWeapons; ++i) weapons[i]->draw();

	drawFilledRect(infoBox, {50,0,0,1});

	al_draw_bitmap(bots[0].bitmap, 50, 50, 0);
	al_draw_bitmap(missileBitmap, 300, 300, 0);
	al_draw_bitmap(laserBitmap, 50, 300, 0);

	al_flip_display();
}

//al_draw_arc(50, 50, 100, 0, RAD_PER_DEG *90, {0,0,50,0.5}, 200); draws circle sector