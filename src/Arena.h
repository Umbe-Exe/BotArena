#pragma once
#ifndef ARENA_STATIC
    #ifdef _WIN32
        #ifdef EXPORTS
            #define ARENA_EXPORT __declspec(dllexport)
        #else
            #define ARENA_EXPORT __declspec(dllimport)
        #endif
    #else
        #define ARENA_EXPORT
    #endif
#else
    #define ARENA_EXPORT
#endif

enum COLOR {
	RED,
	BLUE,
	GREEN,
	YELLOW,
	RANDCOL
};

void ARENA_EXPORT start();
void ARENA_EXPORT end();

//manager() will be called every iteration of the game loop
void ARENA_EXPORT appointManager(void (*manager)(), bool startPaused = 1);

//The image background is considered as the color rgb(255, 0, 255)
void ARENA_EXPORT registerBot(const char *name, COLOR color, const char *img, void (*updateFn)(double), void (*initFn)(), void (*deathSignal)() = nullptr);