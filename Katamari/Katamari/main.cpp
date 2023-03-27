
#include "Core/Game.h"
#include "Core/GameObject.h"

#include "Core/Components/Camera.h"
#include "Core/Components/LightComponent.h"

#include "Core/Components/RendererComponents/GridComponent.h"
#include "Core/Components/RendererComponents/PlaneComponent.h"

#include "Core/Components/RendererComponents/SphereComponent.h"
#include "Core/Components/RendererComponents/MeshComponent.h"

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

		auto light = new GameObject("Light");
		std::cout << light->AddComponent(new LightComponent()) << std::endl;

		auto grid = new GameObject("Grid");
		//std::cout << grid->AddComponent(new GridComponent()) << std::endl;
		auto plane = new PlaneComponent(50.0f);
		std::cout << grid->AddComponent(plane) << std::endl;

		auto player = new GameObject("Player");
		std::cout << player->AddComponent(new KatamariBallController()) << std::endl;

		auto ball = new GameObject("Ball");
		std::cout << ball->AddComponent(new KatamariBall()) << std::endl;
		ball->Transform->SetParent(player->Transform);
		ball->Transform->Position.y = 1.0f;

		camera->Transform->SetParent(player->Transform);
		camera->Transform->Position = Vector3::Backward * 10.0f + Vector3::Up * 5.0f;

		MyGame->AddGameObject(camera);
		MyGame->AddGameObject(light);
		MyGame->AddGameObject(grid);
		MyGame->AddGameObject(player);
		MyGame->AddGameObject(ball);

		for (int i = -3; i <= 3; i++)
		{
			auto mesh = new GameObject("Item");
			mesh->Transform->Position = Vector3::Forward * 14.0f + Vector3::Right * i * 2;
			std::cout << mesh->AddComponent(new MeshComponent("Models/hamburger.obj", "Textures/hamburger.dds")) << std::endl;
			std::cout << mesh->AddComponent(new KatamariItem(0.6f)) << std::endl;


			MyGame->AddGameObject(mesh);
		}


		MyGame->Run();
	}

	MyGame->Exit();
	delete MyGame;

	return 0;
}