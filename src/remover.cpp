#include "remover.h"
#include "infobox.h"
#include <set>

std::set<uint8_t> bot;
std::set<uint8_t> weapon;
std::vector<const char *> terminatedBots;

void addBotToDestroy(Bot *bot) {
	uint8_t i = 0;
	while(bots[i] != bot) ++i;
	::bot.insert(i);
}

void addWeaponToDestroy(Weapon *weapon) {
	uint8_t i = 0;
	while(weapons[i] != weapon) ++i;
	::weapon.insert(i);
}

void destroyTheStuff() {

	if(bot.size()) {
		for(auto it = bot.rbegin(); it != bot.rend(); it++) {
			terminatedBots.push_back(bots[*it]->name);
			bots.erase(bots.begin() + *it);
		}

		bot.clear();
		primeInfoboxBitmap();
		al_set_target_backbuffer(window);
	}

	if(weapon.size()) {
		for(auto it = weapon.rbegin(); it != weapon.rend(); it++) weapons.erase(weapons.begin() + *it);
		weapon.clear();
	}
}
