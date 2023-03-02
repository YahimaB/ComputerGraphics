#include "Game.h"


int main()
{
	Game* MyGame = new Game(L"MySuperApp");

	MyGame->CreateComponent(new ShapeComponent());
	MyGame->CreateComponent(new ShapeComponent());
	MyGame->CreateComponent(new ShapeComponent());
	MyGame->CreateComponent(new ShapeComponent());


	if (MyGame->Initialize())
	{
		MyGame->Run();
	}

	MyGame->Exit();
	delete MyGame;

	return 0;
}