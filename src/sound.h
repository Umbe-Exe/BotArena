#pragma once
#ifdef SOUND

struct Sound {
	Sound();
	~Sound();
};

enum class SoundType {
	LASER, LASERHIT, MISSILE,
	MISSILEHIT, BOTCOLLISION, BOTEXPLOSION
};

void playSound(SoundType sound);

#endif