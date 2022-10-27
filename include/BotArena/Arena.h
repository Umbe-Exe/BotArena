#pragma once
#include "competition.h"
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

class ARENA_EXPORT Arena {
public:
    Arena(const char *title = "Arena", const char *filename = "config.txt");
    ~Arena();

    void registerController(Controller *controller);

    void start();

    void appointManager(void (*manager)());
    void onExitCallback(void (*onExit)());

    void end();
    void pause();
    void unpause();
    void enableFeedback();
    void disableFeedback();
    void setSpeed(double speed);

private:
    Arena_Impl *pimpl;
    friend class Controller;
};