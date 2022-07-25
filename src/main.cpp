#include "../demo_bots/Umbe.h"
#include "../demo_bots/Umbe1.h"
#include "../demo_bots/Umbe2.h"
#include "../demo_bots/Umbe3.h"
#include "../demo_bots/Umbe4.h"
#include "../demo_bots/Umbe5.h"
#include "../demo_bots/rammingBot.h"

int main() {
	registerUmbe();
	registerUmbe1();
	registerUmbe2();
	registerUmbe3();
	registerUmbe4();
	registerUmbe5();
	
	registerRam();
	registerRam();
	start();
}