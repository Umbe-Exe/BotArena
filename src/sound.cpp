#ifdef SOUND
#include "sound.h"
#include <allegro5/allegro_audio.h>
#include <allegro5/allegro_acodec.h>

ALLEGRO_SAMPLE *laserSound;
ALLEGRO_SAMPLE *laserHitSound;
ALLEGRO_SAMPLE *missileSound;
ALLEGRO_SAMPLE *missileHitSound;
ALLEGRO_SAMPLE *botCollisionSound;
ALLEGRO_SAMPLE *botExplosionSound;

Sound sound;

void loadSounds() {
	al_reserve_samples(100);

	laserSound = al_load_sample("resources/sounds/laser.wav");
	laserHitSound = al_load_sample("resources/sounds/laserhit.wav");
	missileSound = al_load_sample("resources/sounds/missile.wav");
	missileHitSound = al_load_sample("resources/sounds/missilehit.wav");
	botCollisionSound = al_load_sample("resources/sounds/robotscollide.wav");
	botExplosionSound = al_load_sample("resources/sounds/robotexplode.wav");
}

Sound::Sound() {
	al_init();
	al_install_audio();
	al_init_acodec_addon();
	loadSounds();
}

Sound::~Sound() {
	al_uninstall_audio();
	al_uninstall_system();
}

void playSound(SoundType sound) {
	switch(sound) {
		case SoundType::LASER:
			al_play_sample(laserSound, 0.5f, 0.f, 1.f, ALLEGRO_PLAYMODE_ONCE, 0);
			break;
		case SoundType::LASERHIT:
			al_play_sample(laserHitSound, 1.f, 0.f, 1.f, ALLEGRO_PLAYMODE_ONCE, 0);
			break;
		case SoundType::MISSILE:
			al_play_sample(missileSound, 0.3f, 0.f, 1.f, ALLEGRO_PLAYMODE_ONCE, 0);
			break;
		case SoundType::MISSILEHIT:
			al_play_sample(missileHitSound, 0.3f, 0.f, 1.f, ALLEGRO_PLAYMODE_ONCE, 0);
			break;
		case SoundType::BOTCOLLISION:
			al_play_sample(botCollisionSound, 0.1f, 0.f, 1.f, ALLEGRO_PLAYMODE_ONCE, 0);
			break;
		case SoundType::BOTEXPLOSION:
			al_play_sample(botExplosionSound, 0.6f, 0.f, 1.f, ALLEGRO_PLAYMODE_ONCE, 0);
			break;
		default:
			break;
	}
}

#endif