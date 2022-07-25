#include "Arena.h"
#include "../test_bot/test_bot.h"

int main() {
	registerBot("A", RANDCOL, "resources/images/Coke.bmp", test_actions, test_init);
	registerBot("B", RANDCOL, "resources/images/Coke.bmp", test_actions, test_init);
	registerBot("C", RANDCOL, "resources/images/Coke.bmp", test_actions, test_init);
	registerBot("D", RANDCOL, "resources/images/Coke.bmp", test_actions, test_init);
	registerBot("E", RANDCOL, "resources/images/Coke.bmp", test_actions, test_init);
	registerBot("F", RANDCOL, "resources/images/Coke.bmp", test_actions, test_init);
	registerBot("G", RANDCOL, "resources/images/Coke.bmp", test_actions, test_init);
	registerBot("H", RANDCOL, "resources/images/Coke.bmp", test_actions, test_init);
	registerBot("I", RANDCOL, "resources/images/Coke.bmp", test_actions, test_init);
	registerBot("J", RANDCOL, "resources/images/Coke.bmp", test_actions, test_init);
	registerBot("K", RANDCOL, "resources/images/Coke.bmp", test_actions, test_init);
	registerBot("L", RANDCOL, "resources/images/Coke.bmp", test_actions, test_init);
	registerBot("M", RANDCOL, "resources/images/Coke.bmp", test_actions, test_init);
	registerBot("N", RANDCOL, "resources/images/Coke.bmp", test_actions, test_init);
	registerBot("O", RANDCOL, "resources/images/Coke.bmp", test_actions, test_init);
	registerBot("P", RANDCOL, "resources/images/Coke.bmp", test_actions, test_init);

	start();
}