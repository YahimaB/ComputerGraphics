#include "Core/Game.h"
#include "ShapeComponents/DelimiterShapeComponent.h"
#include "ShapeComponents/PlatformShapeComponent.h"
#include "ShapeComponents/CircleShapeComponent.h"
#include "ShapeComponents/SuperShapeComponent.h"

int main()
{
	Game* MyGame = new Game(L"MySuperApp");

	PlatformShapeComponent* Platforms[2] = { new PlatformShapeComponent(-0.95f), new PlatformShapeComponent(0.95f) };

	MyGame->CreateComponent(new DelimiterShapeComponent());
	MyGame->CreateComponent(Platforms[0]);
	MyGame->CreateComponent(Platforms[1]);
	MyGame->CreateComponent(new CircleShapeComponent(0.05f, Platforms));

	/*MyGame->CreateComponent(new SuperShapeComponent());*/

	if (MyGame->Initialize())
	{
		MyGame->Run();
	}

	MyGame->Exit();
	delete MyGame;

	return 0;
}