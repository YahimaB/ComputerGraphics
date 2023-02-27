#include "Game.h"


int main()
{
	Game* MyGame = new Game(L"MySuperApp");

	if (MyGame->Initialize())
	{
		MyGame->Run();
	}

	MyGame->Exit();
	delete MyGame;

	return 0;
}