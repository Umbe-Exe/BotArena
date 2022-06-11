#include "update.h"
#include "utils.h"
#include <allegro5/allegro.h>
#include <allegro5/allegro_primitives.h>

void logic(double delta) {



}

void draw() {

	Rect rect{{0.75,0},{1,1}};

	drawFilledRect(rect, {50,0,0,1});

	

	al_flip_display();
}

//al_draw_arc(50, 50, 100, 0, RAD_PER_DEG *90, {0,0,50,0.5}, 200); draws circle sector