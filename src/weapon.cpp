#include "weapon.h"
#include "data.h"

void Missile::draw() {

	al_draw_rotated_bitmap(missileBitmap,
			       al_get_bitmap_width(missileBitmap) / 2,
			       al_get_bitmap_height(missileBitmap) / 2,
			       coord.x * getSmallestSide(), coord.y * getSmallestSide(), heading, 0);
}

void Missile::update(double delta) {

	for(uint8_t i = 0; i < nOfBots; ++i)
		if(getDistance(coord, bots[i].coord) < botRadius + weaponRadius) {
			if(bots[i].shield > shieldLeakLevel) bots[i].shield -= missileDamage;
			else {
				uint8_t generatorDamage = missileDamage - missileDamage / 100 * bots[i].shield;
				bots[i].shield -= missileDamage - generatorDamage;
				bots[i].energy -= generatorDamage;
			}

			if(bots[i].shield < 0) bots[i].shield = 0;
			if(bots[i].energy <= 0); //subscribe bot to entity destroyer

			for(uint8_t j = 0; j < nOfBots; ++j)
				if(j != i && getDistance(coord, bots[j].coord) < missileBlastRadius + botRadius) {

					uint8_t blastDamage = missileDamage - missileDamage * (getDistance(coord, bots[j].coord) / (missileBlastRadius + botRadius));

					if(bots[i].shield > shieldLeakLevel)
						bots[i].shield -= blastDamage;
					else {
						uint8_t generatorDamage = blastDamage - blastDamage / 100 * bots[i].shield;
						bots[i].shield -= blastDamage - generatorDamage;
						bots[i].energy -= generatorDamage;
					}

					if(bots[i].shield < 0) bots[i].shield = 0;
					if(bots[i].energy <= 0); //subscribe bot to entity destroyer
				}
			//subscribe weapon to entity destroyer
		}

	coord.x += missileSpeed * cos(heading * RAD_PER_DEG);
	coord.y += missileSpeed * sin(heading * RAD_PER_DEG);
}

void Laser::draw() {

	al_draw_rotated_bitmap(laserBitmap,
			       al_get_bitmap_width(laserBitmap) / 2,
			       al_get_bitmap_height(laserBitmap) / 2,
			       coord.x * getSmallestSide(), coord.y * getSmallestSide(), heading, 0);
}

void Laser::update(double delta) {

	for(uint8_t i = 0; i < nOfBots; ++i)
		if(getDistance(coord, bots[i].coord) < botRadius + weaponRadius) {
			if(bots[i].shield > shieldLeakLevel) bots[i].shield -= damage;
			else {
				uint8_t generatorDamage = damage - damage / 100 * bots[i].shield;
				bots[i].shield -= damage - generatorDamage;
				bots[i].energy -= generatorDamage;
			}

			if(bots[i].shield < 0) bots[i].shield = 0;
			if(bots[i].energy <= 0); //subscribe bot to entity destroyer

			//subscribe weapon to entity destroyer
		}

	coord.x += missileSpeed * cos(heading * RAD_PER_DEG);
	coord.y += missileSpeed * sin(heading * RAD_PER_DEG);
}
