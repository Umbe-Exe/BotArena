#include "Arena.h"
#include "../test_bot/test_bot.h"

int main() {
	registerBot("test_bot", RANDCOL, "resources/images/Coke.bmp", test_actions, test_init);
	registerBot("test_bot", RANDCOL, "resources/images/Coke.bmp", test_actions, test_init);
	registerBot("test_bot", RANDCOL, "resources/images/Coke.bmp", test_actions, test_init);
	registerBot("test_bot", RANDCOL, "resources/images/Coke.bmp", test_actions, test_init);
	registerBot("test_bot", RANDCOL, "resources/images/Coke.bmp", test_actions, test_init);
	registerBot("test_bot", RANDCOL, "resources/images/Coke.bmp", test_actions, test_init);

	start();
}