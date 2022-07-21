#include "weapon.h"
#include "data.h"

void Missile::draw() {

	al_draw_rotated_bitmap(missileBitmap,
			       al_get_bitmap_width(missileBitmap) / 2,
			       al_get_bitmap_height(missileBitmap) / 2,
			       coord.x * arenaSize, coord.y * arenaSize, DEG_PER_RAD * heading, 0);
}

void Missile::update(double delta) {

	for(Bot *bot : bots)
		if(getDistance(coord, bot->coord) < botRadius + weaponRadius) {
			if(bot->shield > shieldLeakLevel) bot->shield -= missileDamage;
			else {
				uint8_t generatorDamage = missileDamage - missileDamage / 100 * bot->shield;
				bot->shield -= missileDamage - generatorDamage;
				bot->energy -= generatorDamage;
			}

			if(bot->shield < 0) bot->shield = 0;
			if(bot->energy <= 0); //subscribe bot to entity destroyer

			for(Bot *botAround : bots)
				if(botAround != bot && getDistance(coord, botAround->coord) < missileBlastRadius + botRadius) {

					uint8_t blastDamage = missileDamage - missileDamage * (getDistance(coord, botAround->coord) / (missileBlastRadius + botRadius));

					if(botAround->shield > shieldLeakLevel)
						botAround->shield -= blastDamage;
					else {
						uint8_t generatorDamage = blastDamage - blastDamage / 100 * botAround->shield;
						botAround->shield -= blastDamage - generatorDamage;
						botAround->energy -= generatorDamage;
					}

					if(botAround->shield < 0) botAround->shield = 0;
					if(botAround->energy <= 0); //subscribe bot to entity destroyer
				}
			//subscribe weapon to entity destroyer
		}

	coord.x += missileSpeed * cosf(heading * DEG_PER_RAD) * delta;
	coord.y += missileSpeed * sinf(heading * DEG_PER_RAD) * delta;

	if(
		coord.x < battleBox.topLeft.x + weaponRadius ||
		coord.x > battleBox.bottomRight.x - weaponRadius ||
		coord.y < battleBox.topLeft.y + weaponRadius ||
		coord.y > battleBox.bottomRight.y - weaponRadius)
		;//subscribe weapon to entity destroyer
}

void Laser::draw() {

	al_draw_rotated_bitmap(laserBitmap,
			       al_get_bitmap_width(laserBitmap) / 2,
			       al_get_bitmap_height(laserBitmap) / 2,
			       coord.x * arenaSize, coord.y * arenaSize, DEG_PER_RAD * heading, 0);
}

void Laser::update(double delta) {

	for(Bot *bot : bots)
		if(getDistance(coord, bot->coord) < botRadius + weaponRadius) {
			if(bot->shield > shieldLeakLevel) bot->shield -= damage;
			else {
				uint8_t generatorDamage = damage - damage / 100 * bot->shield;
				bot->shield -= damage - generatorDamage;
				bot->energy -= generatorDamage;
			}

			if(bot->shield < 0) bot->shield = 0;
			if(bot->energy <= 0); //subscribe bot to entity destroyer

			//subscribe weapon to entity destroyer
		}

	coord.x += missileSpeed * cosf(heading * DEG_PER_RAD) * delta;
	coord.y += missileSpeed * sinf(heading * DEG_PER_RAD) * delta;

	if(
		coord.x < battleBox.topLeft.x + weaponRadius ||
		coord.x > battleBox.bottomRight.x - weaponRadius ||
		coord.y < battleBox.topLeft.y + weaponRadius ||
		coord.y > battleBox.bottomRight.y - weaponRadius)
		;//subscribe weapon to entity destroyer
}
