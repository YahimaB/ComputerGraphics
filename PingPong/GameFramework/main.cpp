#include "Game.h"
#include "DelimiterShapeComponent.h"
#include "PlatformShapeComponent.h"
#include "CircleShapeComponent.h"

int main()
{
	Game* MyGame = new Game(L"MySuperApp");

	MyGame->CreateComponent(new DelimiterShapeComponent());
	MyGame->CreateComponent(new PlatformShapeComponent(-0.9f));
	MyGame->CreateComponent(new PlatformShapeComponent(0.9f));
	MyGame->CreateComponent(new CircleShapeComponent(0.05f));


	if (MyGame->Initialize())
	{
		MyGame->Run();
	}

	MyGame->Exit();
	delete MyGame;

	return 0;
}