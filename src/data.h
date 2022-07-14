#pragma once

#include "bot.h"
#include "weapon.h"
#define infoBox {{0.75,0},{1,1}}

extern Rect battleBox;

#define botRadius 0.05f
#define weaponRadius 0.015f
#define radarMaxRange 0.15f
#define rangeMaxRange 0.2f
#define sensorEnergyConsumption 2

#define laserDamageMoltiplicator 0.05f // already normalized on the 100% laser system level (100 * 0.05 = 5 max damage)
#define missileBlastRadius 0.1f
#define missileDamage 10
#define laserSpeed 0.15f
#define missileSpeed 0.08f

#define shieldLeakLevel 80

#define bumpForce 0.05f
#define friction 0.02f // friction applied after bump

extern std::vector<Bot *> bots;
extern Bot *currBot;

extern ALLEGRO_BITMAP *missileBitmap, *laserBitmap;

extern std::vector<Weapon *> weapons;

extern ALLEGRO_DISPLAY *window;
extern int arenaSize;

/*
IMPORTANT INFO
the user will deal with percentages only
and degree angles

screen is represented by normalized coordinates 0..1
damage, energy consumptions and system levels are still percentages

*/