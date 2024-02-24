#include "Game.h"
#include <iostream>

int main(int argc, char** argv)
{
	Game game;

	bool success = game.Initialize();

	if (success)
	{
		std::cout << "Running Game" << std::endl;
		game.RunLoop();
	}

	game.Shutdown();

	return 0;
}