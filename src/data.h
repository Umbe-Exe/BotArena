#pragma once
#include "bot.h"
#include "weapon.h"
#include "utils.h"

extern Rect battleBox;

extern std::vector<Bot *> bots;
extern Bot *currBot;

extern ALLEGRO_BITMAP *missileBitmap, *laserBitmap;

extern std::vector<Weapon *> weapons;

extern ALLEGRO_DISPLAY *window;
extern int win_w, win_h;
extern float arenaSize;

extern std::vector<const char *> terminatedBots;