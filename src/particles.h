#pragma once
#include "weapon.h"
#include <allegro5/allegro.h>

void AddParticle(float x, float y, ALLEGRO_COLOR color, float heading, float speed, float timeToLive);

void updateParticles(double delta);

void drawPrticles();


void createMissileParticleTrail(Missile *missile);

void createBotCollisionBurst(float x, float y);

void createBotExplosionBurst(float x, float y);