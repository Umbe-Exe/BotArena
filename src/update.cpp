#include "update.h"
#include "data.h"

void logic(double delta) {

	for(Bot *bot : bots) bot->update(delta);
	for(Weapon *weapon : weapons) weapon->update(delta);

}

void draw() {

	al_clear_to_color(al_map_rgb(200, 200, 200));

	for(Bot *bot : bots) bot->draw();
	for(Weapon *weapon : weapons) weapon->draw();

	drawFilledRect(infoBox, {50,0,0,1});

	drawRect(battleBox, {255,255,255,255}, 1);

	al_flip_display();
};