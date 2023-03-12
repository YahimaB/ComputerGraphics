
#include "Core/Game.h"
#include "Core/GameObject.h"

#include "Core/Components/Camera.h"

#include "Core/Components/RendererComponents/GridComponent.h"
#include "Core/Components/RendererComponents/SphereComponent.h"

#include "KatamariScripts/KatamariBall.h"
#include "KatamariScripts/KatamariBallController.h"
#include "KatamariScripts/KatamariCameraController.h"


int main()
{
	Game* MyGame = new Game(L"Katamari");

	if (MyGame->Initialize())
	{
		auto camera = new GameObject("Camera");
		std::cout << camera->AddComponent(new Camera()) << std::endl;
		std::cout << camera->AddComponent(new KatamariCameraController()) << std::endl;

		auto grid = new GameObject("Grid");
		std::cout << grid->AddComponent(new GridComponent()) << std::endl;

		auto player = new GameObject("Player");
		std::cout << player->AddComponent(new KatamariBallController()) << std::endl;

		auto ball = new GameObject("Ball");
		std::cout << ball->AddComponent(new KatamariBall()) << std::endl;
		ball->Transform->SetParent(player->Transform);

		camera->Transform->SetParent(player->Transform);
		camera->Transform->Position = Vector3::Backward * 10.0f + Vector3::Up * 5.0f;


		auto sphere2 = new GameObject("Sphere");
		std::cout << sphere2->AddComponent(new SphereComponent(0.5f)) << std::endl;
		sphere2->Transform->SetParent(player->Transform);
		sphere2->Transform->Position = Vector3::Right * 5.0f + Vector3::Up * 1.0f;


		MyGame->AddGameObject(camera);
		MyGame->AddGameObject(grid);
		MyGame->AddGameObject(player);
		MyGame->AddGameObject(ball);

		MyGame->AddGameObject(sphere2);

		MyGame->Run();
	}

	MyGame->Exit();
	delete MyGame;

	return 0;
}