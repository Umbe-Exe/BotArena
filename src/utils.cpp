#include "arena_impl.h"

void Arena_Impl::transposeEntities(Coord vector) {
	for(Bot &bot : bots) bot.coord += vector;
	for(Weapon *weapon : weapons) weapon->coord += vector;
}

void Arena_Impl::makeBattleBox() {
	if(window) {
		win_w = al_get_display_width(window);
		win_h = al_get_display_height(window);

		if(infoBox.topLeft.x * win_w < win_h) arenaSize = infoBox.topLeft.x * win_w;
		else arenaSize = win_h;

		Coord location = {(infoBox.topLeft.x / 2.f * win_w - arenaSize / 2.f) / arenaSize,(win_h / 2.f - arenaSize / 2.f) / arenaSize};

		transposeEntities(location - battleBox.topLeft);

		battleBox = {location,location + Coord{1,1}};
	} else battleBox = {{0,0},{1,1}};
}