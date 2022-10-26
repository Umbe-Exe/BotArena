#include "arena_impl.h"

Arena::Arena(const char *title, const char *filename) {
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
