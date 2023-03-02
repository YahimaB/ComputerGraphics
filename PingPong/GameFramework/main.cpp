#include "Game.h"
#include "SuperShapeComponent.h"

int main()
{
	Game* MyGame = new Game(L"MySuperApp");

	MyGame->CreateComponent(new SuperShapeComponent());
	MyGame->CreateComponent(new SuperShapeComponent());
	MyGame->CreateComponent(new SuperShapeComponent());
	MyGame->CreateComponent(new SuperShapeComponent());


	if (MyGame->Initialize())
	{
		MyGame->Run();
	}

	MyGame->Exit();
	delete MyGame;

	return 0;
}