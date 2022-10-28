#pragma once
#include "arena_impl.h"

struct AllegroSystem {

	std::mutex m;

	AllegroSystem() {
		al_init();
		al_init_primitives_addon();
		al_init_image_addon();
		al_init_font_addon();
		al_init_ttf_addon();
		al_install_audio();
		al_init_acodec_addon();
		al_install_keyboard();
		al_install_mouse();

#ifdef SOUND
		loadSounds();
#endif
#ifdef PARTICLES
		loadParticles();
#endif
	}
#ifdef PARTICLES
	~AllegroSystem() {
		unloadParticles();
	}
#endif
};