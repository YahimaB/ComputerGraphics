#include "Game.h"
#include "PlatformShapeComponent.h"

int main()
{
	Game* MyGame = new Game(L"MySuperApp");

	MyGame->CreateComponent(new PlatformShapeComponent());
	/*MyGame->CreateComponent(new PlatformShapeComponent());
	MyGame->CreateComponent(new PlatformShapeComponent());
	MyGame->CreateComponent(new PlatformShapeComponent());*/


	if (MyGame->Initialize())
	{
		MyGame->Run();
	}

	MyGame->Exit();
	delete MyGame;

	return 0;
}