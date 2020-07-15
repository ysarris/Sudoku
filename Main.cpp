#include "Game.h"

int main()
{
	// Seed random number generator with current time
	srand((unsigned int)time(0));

	// Start game
	Game::Instance().Run();

	return EXIT_SUCCESS;
}