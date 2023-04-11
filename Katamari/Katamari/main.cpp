
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

		auto dirLight = new GameObject("Light");
		std::cout << dirLight->AddComponent(new LightComponent(0, Vector3::One * 0.5f)) << std::endl;
		dirLight->Transform->Rotation = Quaternion::CreateFromAxisAngle(Vector3::Left, XM_PI - XM_PI / 1.5f);

		auto pointLight1 = new GameObject("Light");
		std::cout << pointLight1->AddComponent(new LightComponent(1, Vector3(10.0f, 0.0f, 0.0f))) << std::endl;
		pointLight1->Transform->Position = Vector3::Forward * 30.0f + Vector3::Up * 10.0f + Vector3::Right * 35.0f;

		auto pointLight2 = new GameObject("Light");
		std::cout << pointLight2->AddComponent(new LightComponent(1, Vector3(0.0f, 0.0f, 10.0f))) << std::endl;
		pointLight2->Transform->Position = Vector3::Forward * 30.0f + Vector3::Up * 10.0f - Vector3::Right * 35.0f;

		auto spotLight = new GameObject("Light");
		std::cout << spotLight->AddComponent(new LightComponent(2, Vector3(0.0f, 10.0f, 0.0f))) << std::endl;
		spotLight->Transform->Position = Vector3::Forward * 10.0f + Vector3::Up * 10.0f + Vector3::Right * 0.0f;

		auto grid = new GameObject("Grid");
		//std::cout << grid->AddComponent(new GridComponent()) << std::endl;
		auto plane = new PlaneComponent(200.0f);
		std::cout << grid->AddComponent(plane) << std::endl;

		auto player = new GameObject("Player");
		std::cout << player->AddComponent(new KatamariBallController()) << std::endl;

		auto ball = new GameObject("Ball");
		std::cout << ball->AddComponent(new KatamariBall(1.5)) << std::endl;
		ball->Transform->SetParent(player->Transform);
		ball->Transform->Position.y = 1.6f;

		camera->Transform->SetParent(player->Transform);
		camera->Transform->Position = Vector3::Backward * 10.0f + Vector3::Up * 5.0f;

		MyGame->AddGameObject(camera);
		MyGame->AddGameObject(dirLight);
		MyGame->AddGameObject(pointLight1);
		MyGame->AddGameObject(pointLight2);
		MyGame->AddGameObject(spotLight);
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

		auto mesh = new GameObject("BigBoy");
		mesh->Transform->Position = Vector3::Forward * 30.0f;
		mesh->Transform->Scale = Vector3::One * 30.0f;
		std::cout << mesh->AddComponent(new MeshComponent("Models/hamburger.obj", "Textures/hamburger.dds")) << std::endl;
		MyGame->AddGameObject(mesh);

		MyGame->Run();
	}

	MyGame->Exit();
	delete MyGame;

	return 0;
}