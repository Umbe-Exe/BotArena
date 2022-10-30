#pragma once
#ifdef SOUND
#include <allegro5/allegro_audio.h>
#include <allegro5/allegro_acodec.h>

void loadSounds();

enum class SoundType {
	LASER, LASERHIT, MISSILE,
	MISSILEHIT, BOTCOLLISION, BOTEXPLOSION
};

void playSound(SoundType sound);

#endif