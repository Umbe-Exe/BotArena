#include "../demo_bots/rammingBot.h"
#include "../demo_bots/Umbe.h"
#include "Arena.h"
#include <thread>

void arena1() {
    Arena arena("Arena1");

    arena.registerController(new Rammer());
    arena.registerController(new Rammer());
    arena.registerController(new Rammer());
    arena.registerController(new Umbe());
    arena.registerController(new Umbe());
    arena.registerController(new Umbe());

    arena.start();
}

void arena2() {
    Arena arena("Arena2");

    arena.registerController(new Rammer());
    arena.registerController(new Rammer());
    arena.registerController(new Rammer());
    arena.registerController(new Rammer());
    arena.registerController(new Rammer());
    arena.registerController(new Rammer());
    arena.registerController(new Rammer());
    arena.registerController(new Rammer());
    arena.registerController(new Rammer());
    arena.registerController(new Rammer());
    arena.registerController(new Rammer());
    arena.registerController(new Rammer());
    arena.registerController(new Rammer());
    arena.registerController(new Rammer());
    arena.registerController(new Rammer());

    arena.start();
}

int main() {

    std::thread t1(arena1);
    std::thread t2(arena2);

    Arena arena("Arena1");


    arena.registerController(new Umbe());
    arena.registerController(new Umbe());
    arena.registerController(new Umbe());
    arena.registerController(new Umbe());
    arena.registerController(new Umbe());

    arena.start();

    t1.join();
    t2.join();
}
