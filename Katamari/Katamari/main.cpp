
#include "Core/Game.h"
#include "Core/GameObject.h"

#include "Core/Components/Camera.h"

#include "Core/Components/RendererComponents/GridComponent.h"
#include "Core/Components/RendererComponents/SphereComponent.h"

#include "KatamariScripts/KatamariBall.h"
#include "KatamariScripts/KatamariBallController.h"
#include "KatamariScripts/KatamariCameraController.h"
#include "KatamariScripts/KatamariItem.h"


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


		auto item = new GameObject("Item");
		item->Transform->Position = Vector3::Forward * 7.0f;
		std::cout << item->AddComponent(new KatamariItem()) << std::endl;

		auto item1 = new GameObject("Item");
		item1->Transform->Position = Vector3::Right * 7.0f;
		std::cout << item1->AddComponent(new KatamariItem()) << std::endl;

		auto item2 = new GameObject("Item");
		item2->Transform->Position = Vector3::Left * 7.0f;
		std::cout << item2->AddComponent(new KatamariItem()) << std::endl;

		MyGame->AddGameObject(camera);
		MyGame->AddGameObject(grid);
		MyGame->AddGameObject(player);
		MyGame->AddGameObject(ball);

		MyGame->AddGameObject(item);
		MyGame->AddGameObject(item1);
		MyGame->AddGameObject(item2);

		MyGame->Run();
	}

	MyGame->Exit();
	delete MyGame;

	return 0;
}