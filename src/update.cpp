#include "update.h"
#include "data.h"
#include "infobox.h"
#include "remover.h"
#include "particles.h"

void logic(double delta) {

	for(Bot *bot : bots) bot->update(delta);
	for(Weapon *weapon : weapons) weapon->update(delta);

	destroyTheStuff();

	if(allowParticles) updateParticles(delta);
}

void draw() {

	al_clear_to_color(al_map_rgb(200, 200, 200));

	for(Bot *bot : bots) bot->draw();
	for(Weapon *weapon : weapons) weapon->draw();

	if(allowParticles) drawPrticles();

	drawRect(battleBox, {255,255,255,255}, 1);

	drawInfobox();

	al_flip_display();
};