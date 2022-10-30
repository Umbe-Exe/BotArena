#include "arena_impl.h"
#include <iostream>

void initAllegroOnce() {
	al_init();
	al_init_primitives_addon();
	al_init_image_addon();
	al_init_font_addon();
	al_init_ttf_addon();
	al_install_keyboard();
	al_install_mouse();

#ifdef SOUND
	al_init_acodec_addon();
	al_install_audio();
	loadSounds();
#endif
#ifdef PARTICLES
	loadParticles();
#endif
}

std::once_flag allegroInitFlag;

Arena::Arena(const char *title, const char *filename) {
	std::call_once(allegroInitFlag, initAllegroOnce);

	pimpl = new Arena_Impl;

	pimpl->loadConfigFile(filename);
	pimpl->title = title;
}

Arena::~Arena() {

	if(pimpl->onExit) pimpl->onExit();

	pimpl->destroyWeapons();
	pimpl->destroyInfobox();

	delete pimpl;
}

void Arena::start() {
	std::thread thread((* pimpl));
	thread.join();
}

void Arena::appointManager(void(*manager)()) {
	pimpl->manager = manager;
}

void Arena::onExitCallback(void(*onExit)()) {
	pimpl->onExit = onExit;
}

void Arena::end() {
	pimpl->running = 0;
}

void Arena::pause() {
	pimpl->pause = 1;
}

void Arena::unpause() {
	pimpl->pause = 0;
}

void Arena::enableFeedback() {
	pimpl->feedback(1);
}

void Arena::disableFeedback() {
	pimpl->feedback(0);
}

void Arena::setSpeed(double speed) {
	pimpl->speed = speed;
}

void Arena::registerController(Controller *controller) {
	pimpl->controllers.push_back(controller);
}
