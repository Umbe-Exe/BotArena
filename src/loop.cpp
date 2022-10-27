#include "arena_impl.h"
#include "sync.h"
#include "common.h"

AllegroSystem sync;

void Arena_Impl::operator()() {

	if(allowFeedback) {

		sync.m.lock();
		al_set_new_display_flags(ALLEGRO_RESIZABLE);
		window = al_create_display(500, 300);
		al_set_window_title(window, title);
		queue = al_create_event_queue();

		al_register_event_source(queue, al_get_display_event_source(window));
		al_register_event_source(queue, al_get_keyboard_event_source());
		al_register_event_source(queue, al_get_mouse_event_source());
		sync.m.unlock();

		if(!allowInfobox) infoBox = {{1,1},{1,1}};
	}

	initBots();

	makeBattleBox();
	scatterBots();

	if(allowFeedback) {
		primeBitmaps();
		al_set_target_backbuffer(window);
	}

	ALLEGRO_EVENT event;
	double previous = al_get_time() + 0.00001, current, delta = 0.00001;

	while(running) {

		current = al_get_time();
		delta = current - previous;
		update(delta * speed);
		previous = current;

		if(manager) manager();

		if(allowFeedback) {
			event = {};
			al_get_next_event(queue, &event);

			switch(event.type) {
				case ALLEGRO_EVENT_KEY_DOWN:
					if(event.keyboard.display == window) {
						if(event.keyboard.keycode == ALLEGRO_KEY_S) scatterBots();
						else if(event.keyboard.keycode == ALLEGRO_KEY_P) pause = true;
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
					break;
				case ALLEGRO_EVENT_MOUSE_AXES:
					if(event.mouse.display == window)
						if(event.mouse.dz != 0) infoBoxScroll(event.mouse.dz);
					break;
			}

			while(pause) {
				event = {};
				al_wait_for_event(queue, &event);

				switch(event.type) {
					case ALLEGRO_EVENT_KEY_DOWN:
						if(event.keyboard.display == window) {
							if(event.keyboard.keycode == ALLEGRO_KEY_S) {
								scatterBots();
								update(delta);
								draw();
							} else if(event.keyboard.keycode == ALLEGRO_KEY_P) {
								pause = false;
								previous = al_get_time() + delta;
							} else if(event.keyboard.keycode == ALLEGRO_KEY_UP) {
								delta *= 5;
								update(delta);
								draw();
							} else if(event.keyboard.keycode == ALLEGRO_KEY_DOWN) {
								delta /= 5;
								update(delta);
								draw();
							}
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
						draw();
						break;
					case ALLEGRO_EVENT_MOUSE_AXES:
						if(event.mouse.display == window) {
							if(event.mouse.dz != 0) infoBoxScroll(event.mouse.dz);
							update(delta);
							draw();
						}
						break;
				}
			}
			draw();
		}
	}

	if(allowFeedback) {
		if(window) al_destroy_display(window);
		if(queue) al_destroy_event_queue(queue);		
	}

	for(auto &bot : bots) delete bot.controller;
}

void Arena_Impl::feedback(bool status) {
	if(status != allowFeedback) {
		if(status) {

			sync.m.lock();
			al_init_primitives_addon();
			al_set_new_display_flags(ALLEGRO_RESIZABLE);
			window = al_create_display(500, 300);
			al_set_window_title(window, title);
			queue = al_create_event_queue();

			al_register_event_source(queue, al_get_display_event_source(window));
			al_register_event_source(queue, al_get_keyboard_event_source());
			al_register_event_source(queue, al_get_mouse_event_source());
			sync.m.unlock();

			if(!allowInfobox) infoBox = {{1,1},{1,1}};

			primeBitmaps();
			al_set_target_backbuffer(window);
		} else {
		
			if(window) al_destroy_display(window);
			if(queue) al_destroy_event_queue(queue);

			al_shutdown_primitives_addon();
		}
		allowFeedback = status;
	}
}

void Arena_Impl::update(double delta) {

	for(Bot &bot : bots) bot.update(delta);
	for(Weapon *weapon : weapons) weapon->update(delta);

	destroyBotsAndWeapons();

#ifdef PARTICLES
	if(allowFeedback && allowParticles) updateParticles(delta);
#endif
}

void Arena_Impl::draw() {

	al_clear_to_color(al_map_rgb(200, 200, 200));

	for(Bot &bot : bots) bot.draw();
	for(Weapon *weapon : weapons) weapon->draw();

#ifdef PARTICLES
	if(allowParticles) drawPrticles();
#endif

	drawRect(arenaSize, battleBox, ALLEGRO_COLOR{255,255,255,255}, 1);

	drawInfobox();

	al_flip_display();
};

void Arena_Impl::initBots() {

#pragma warning( suppress : 4311)
#pragma warning( suppress : 4302)
	srand(time(0) + (unsigned int)this);

	bots.reserve(controllers.size());

	ALLEGRO_COLOR color;

	for(auto controller : controllers) {

		switch(controller->color()) {
			case RED:
				color = {255,0,0,1};
				break;
			case BLUE:
				color = {0,0,255,1};
				break;
			case GREEN:
				color = {0,255,0,1};
				break;
			case YELLOW:
				color = {255,255,0,1};
				break;
			case RANDCOL:
			default:
				color = al_map_rgb(rand() % 150, rand() % 150, rand() % 150);
		}

		bots.emplace_back(
			this, controller->name(), controller->pathToImage(), color, controller);

		controller->arena = this;

		currBot = &bots.back();
		controller->onStart();
	}
}