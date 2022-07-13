#include "update.h"
#include "utils.h"
#include <allegro5/allegro.h>
#include <allegro5/allegro_primitives.h>

#include "data.h"

void logic(double delta) {



}

void draw() {

	al_clear_to_color(al_map_rgb(200, 200, 200));

	for(Bot *bot : bots) bot->draw();
	for(Weapon *weapon : weapons) weapon->draw();

	drawFilledRect(infoBox, {50,0,0,1});

	al_flip_display();
};