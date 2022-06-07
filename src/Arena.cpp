#include "Arena.h"
#include "update.h"
#include "utils.h"
#include <allegro5/allegro.h>
#include <allegro5/allegro_primitives.h>

ALLEGRO_DISPLAY *window;
ALLEGRO_EVENT_QUEUE *queue;

void init() {

	al_init();
	al_init_primitives_addon();

	al_set_new_window_title("Arena");
	al_set_new_display_flags(ALLEGRO_RESIZABLE);

	al_set_new_display_option(ALLEGRO_SAMPLE_BUFFERS, 1, ALLEGRO_REQUIRE);
	al_set_new_display_option(ALLEGRO_SAMPLES, 4, ALLEGRO_REQUIRE);

	window = al_create_display(500, 300);

	queue = al_create_event_queue();

	al_install_keyboard();

	al_register_event_source(queue, al_get_display_event_source(window));
	al_register_event_source(queue, al_get_keyboard_event_source()); 
}

void update(double delta) {

	logic(delta);
	
	draw();
}

void display() {

	setHeightWidth(500, 300);

	bool running = true;
	ALLEGRO_EVENT event;
	double previous = al_get_time(), current;

	while(running) {
		al_get_next_event(queue, &event);

		switch(event.type) {
			case ALLEGRO_EVENT_DISPLAY_CLOSE:
				running = false;
				break;
			case ALLEGRO_EVENT_DISPLAY_RESIZE:
				al_acknowledge_resize(window);
				setHeightWidth(al_get_display_width(window), al_get_display_height(window));
				break;
		}

		current = al_get_time();
		update(current - previous);
		previous = current;
	}
}

void destroy() {

	al_destroy_display(window);
	al_destroy_event_queue(queue);
}

void start() {
	init();
	display();
	destroy();
}