#pragma once
#include "Arena.h"
#include "common.h"
#include "utils.h"

#ifdef SOUND
#include "sound.h"
extern Sound sound;
#endif

#ifdef PARTICLES
#include "particles.h"
extern Particles prtcls;
#endif

struct Bot;
struct Weapon;

class Arena_Impl {

public:
	int maxGeneratorStructure = 10000;

	int maxEnergy = 100;

	int maxShield = 10000;

	int shieldLeakLevel = 10000;

	int maxMissile = 200;
	int maxLaser = 100;
	int minLaser = 20;

	float botRadius = 0.05f;
	float weaponRadius = 0.01f;
	float radarMaxRange = 0.15f;
	float rangeMaxRange = 0.2f;
	int sensorEnergyConsumption = 2;

	float laserDamageMoltiplicator = 1.f;
	float missileBlastRadius = 0.1f;
	int missileDamage = 200;
	float laserSpeed = 0.5f;
	float missileSpeed = 0.2f;

	int bumpDamage = 50;
	float bumpForce = 0.2f;
	float friction = 0.2f;
	float maxSpeed = 0.1f;

#ifdef SOUND
	bool allowSound = true;
#endif
#ifdef PARTICLES
	bool allowParticles = true;
#endif
	bool allowInfobox = true;
	bool allowFeedback = true;

	void loadConfigFile(const char *filename);

	ALLEGRO_DISPLAY *window = nullptr;
	ALLEGRO_EVENT_QUEUE *queue = nullptr;
	const char *title;

	void (*manager)() = nullptr;
	void (*onExit)() = nullptr;
	bool running = 1, pause = 0;
	float speed = 1;

	std::vector<Bot> bots;
	Bot *currBot;

	ALLEGRO_BITMAP *missileBitmap = nullptr, *laserBitmap = nullptr;

	std::vector<Weapon *> weapons;

	void destroyWeapons();

	std::set<uint8_t> exploding_bot;
	std::set<uint8_t> exploding_weapon;
	std::vector<const char *> terminatedBots;

	void addBotToDestroy(Bot *bot);
	void addWeaponToDestroy(Weapon *weapon);
	void destroyBotsAndWeapons();

	int win_w, win_h;
	float arenaSize;

	Rect infoBox{{0.75,0},{1,1}};
	Rect battleBox;

	void makeBattleBox();
	void transposeEntities(Coord vector);

#ifdef PARTICLES
	std::vector<Particle> particles;

	void updateParticles(double delta);
	void drawPrticles();
#endif

	ALLEGRO_BITMAP *infoboxBitmap = nullptr;
	ALLEGRO_FONT *nameFont = nullptr, *sysFont = nullptr;

	uint8_t nOfBots;

	uint16_t bitmapHeight, bitmapWidth;
	float scrollPos = 0;

	int nameFontHeight;
	int sysFontHeight;
	int sysFontWidth;

	uint16_t bitmapX;

	void primeInfoboxBitmap();
	void infoBoxScroll(float to);
	void drawInfobox();
	void destroyInfobox();

	void scatterBots();
	void primeBitmaps();

	void update(double delta);
	void draw();

	void operator()();

	void feedback(bool status);

	std::vector<Controller*> controllers;
	void initBots();
};

#include "bot.h"
#include "weapon.h"