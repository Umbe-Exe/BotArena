#pragma once
#include <mutex>
#include "allegro5/allegro.h"

struct AllegroSystem {

	std::mutex m;

	AllegroSystem() {
		al_init();
		al_init_primitives_addon();
		al_init_image_addon();
		al_init_font_addon();
		al_init_ttf_addon();
		al_install_keyboard();
		al_install_mouse();
	}
	~AllegroSystem() {
		al_uninstall_mouse();
		al_uninstall_keyboard();
		al_shutdown_ttf_addon();
		al_shutdown_font_addon();
		al_shutdown_image_addon();
		al_shutdown_primitives_addon();
		al_uninstall_system();
	}
};