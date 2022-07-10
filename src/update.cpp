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

	al_flip_display();
}