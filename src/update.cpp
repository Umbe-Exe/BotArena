#include "update.h"
#include "utils.h"
#include <allegro5/allegro.h>
#include <allegro5/allegro_primitives.h>

void logic(double delta) {



}

void draw() {

	Rect rect{{0.75,0},{1,1}};

	drawRect(rect, {50,0,0,1});

	al_flip_display();
}
