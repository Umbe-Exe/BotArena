#include "Arena.h"
#include "entities.h"
#include "utils.h"
#include "update.h"
#include "infobox.h"
#include "sound.h"
#include "loadConfig.h"
#include <allegro5/allegro_image.h>
#include <allegro5/allegro_primitives.h>
#include <allegro5/allegro_ttf.h>
#include <allegro5/allegro_audio.h>

ALLEGRO_DISPLAY *window = nullptr;
ALLEGRO_EVENT_QUEUE *queue;

std::vector<BotInitData> botsData;

void (*manager)() = nullptr;
bool running, pause = 1;

void appointManager(void (*manager)(), bool startPaused) {
	::manager = manager;
	pause = startPaused;
}

void end() {
	running = 0;
}

void init() {

	al_init();
	al_init_primitives_addon();
	al_init_image_addon();
	al_init_font_addon();
	al_init_ttf_addon();
	
	al_install_audio();

	loadSounds();
	lockParameters();

	al_set_new_window_title("Arena");
	al_set_new_display_flags(ALLEGRO_RESIZABLE);

	al_set_new_display_option(ALLEGRO_SAMPLE_BUFFERS, 1, ALLEGRO_REQUIRE);
	al_set_new_display_option(ALLEGRO_SAMPLES, 4, ALLEGRO_REQUIRE);

	window = al_create_display(500, 300);

	queue = al_create_event_queue();

	al_install_keyboard();
	al_install_mouse();

	al_register_event_source(queue, al_get_display_event_source(window));
	al_register_event_source(queue, al_get_keyboard_event_source());
	al_register_event_source(queue, al_get_mouse_event_source());

	srand(time(0));

	makeBattleBox();

	createBots(botsData);
	botsData.clear();
	botsData.shrink_to_fit();

	scatterBots();
	primeBitmaps();

	al_set_target_backbuffer(window);
}

void update(double delta) {

	logic(delta);
	
	draw();
}

void display() {

	running = true;
	ALLEGRO_EVENT event;
	double previous = al_get_time(), current, delta = 0.00001;

	while(running) {
		event = {};
		al_get_next_event(queue, &event);

		switch(event.type) {
			case ALLEGRO_EVENT_KEY_DOWN:
				if(event.keyboard.keycode == ALLEGRO_KEY_S) scatterBots();
				else if(event.keyboard.keycode == ALLEGRO_KEY_P) pause = true;
				break;
			case ALLEGRO_EVENT_DISPLAY_CLOSE:
				running = false;
				break;
			case ALLEGRO_EVENT_DISPLAY_RESIZE:
				al_acknowledge_resize(window);
				makeBattleBox();
				primeBitmaps(); 
				infoBoxScroll(0);
				al_set_target_backbuffer(window);
				break;
			case ALLEGRO_EVENT_MOUSE_AXES:
				if(event.mouse.dz != 0) infoBoxScroll(event.mouse.dz);
				break;
		}

		while(pause) {
			event = {};
			al_wait_for_event(queue, &event);

			switch(event.type) {
				case ALLEGRO_EVENT_KEY_DOWN:
					if(event.keyboard.keycode == ALLEGRO_KEY_S) {
						scatterBots();
						update(delta);
					} else if(event.keyboard.keycode == ALLEGRO_KEY_P) {
						pause = false;
						previous = al_get_time() + delta;
					} else if(event.keyboard.keycode == ALLEGRO_KEY_UP) {
						delta *= 5;
						update(delta);
					} else if(event.keyboard.keycode == ALLEGRO_KEY_DOWN) {
						delta /= 5;
						update(delta);
					}
					break;
				case ALLEGRO_EVENT_DISPLAY_CLOSE:
					running = false;
					pause = false;
					break;
				case ALLEGRO_EVENT_DISPLAY_RESIZE:
					al_acknowledge_resize(window);
					makeBattleBox();
					primeBitmaps();
					infoBoxScroll(0);
					al_set_target_backbuffer(window);
					update(delta);
					break;
				case ALLEGRO_EVENT_MOUSE_AXES:
					if(event.mouse.dz != 0) infoBoxScroll(event.mouse.dz);
					update(delta);
					break;
			}
		}
		if(manager) manager();

		current = al_get_time();
		update(delta = current - previous);
		previous = current;
	}
}

void destroy() {
	destroyBots();
	destroyWeapons();
	destroyInfobox();
	al_destroy_display(window);
	al_destroy_event_queue(queue);

	al_shutdown_ttf_addon();
	al_shutdown_font_addon();
	al_shutdown_image_addon();
	al_shutdown_primitives_addon();
	al_uninstall_keyboard();
	al_uninstall_mouse();
	al_uninstall_audio();
	al_uninstall_system();
}

void start() {
	init();
	display();
	destroy();
}

void registerBot(const char *name, COLOR color, const char *img, void (*updateFn)(double), void (*initFn)(), void (*deathSignal)()) {
	botsData.push_back({ name, color, img, updateFn, initFn, deathSignal });
}
