#pragma once

#include "bot.h"
#include "weapon.h"
#define infoBox {{0.75,0},{1,1}}
#define battleBox {{0,0},{0.75,1}}
#define botRadius 0.1f
#define weaponRadius 0.02f
#define radarMaxRange 0.5f
#define rangeMaxRange 0.55f
#define sensorEnergyConsumption 2

extern Bot *bots;
extern uint8_t nOfBots;
extern Bot *currBot;

extern ALLEGRO_BITMAP *missileBitmap, *laserBitmap;

extern Weapon *weapons;
extern uint8_t nOfWeapons;


/*
IMPORTANT INFO
the user will deal with percentages only

screen is represented by normalized coordinates 0..1

*/