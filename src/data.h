#pragma once

#include "bot.h"
#include "weapon.h"
#define infoBox {{0.75,0},{1,1}}
#define battleBox {{0,0},{0.75,1}}
#define botRadius 0.05f
#define weaponRadius 0.015f
#define radarMaxRange 0.2f
#define rangeMaxRange 0.25f
#define sensorEnergyConsumption 2

#define laserDamageMoltiplicator 0.05f // already normalized on the 100% laser system level (100 * 0.05 = 5 max damage)
#define missileBlastRadius 0.05f
#define missileDamage 10
#define laserSpeed 0.15f
#define missileSpeed 0.08f

extern Bot *bots;
extern uint8_t nOfBots;
extern Bot *currBot;

extern ALLEGRO_BITMAP *missileBitmap, *laserBitmap;

extern Weapon **weapons;
extern uint8_t nOfWeapons;

/*
IMPORTANT INFO
the user will deal with percentages only
and degree angles

screen is represented by normalized coordinates 0..1
damage, energy consumptions and system levels are still percentages

*/