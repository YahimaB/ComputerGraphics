
#include "Core/Game.h"
#include "Core/GameObject.h"

#include "Core/Components/Camera.h"
#include "Core/Components/OrbitCameraController.h"

#include "ShapeComponents/GridComponent.h"
#include "ShapeComponents/SphereComponent.h"
//#include "ShapeComponents/CosmicBodyComponent.h"
#include "KatamariScripts/KatamariBall.h"
#include "KatamariScripts/KatamariBallController.h"

//SolarSystem
//int main()
//{
//	Game* MyGame = new Game(L"MySuperApp");
//
//	MyGame->CreateComponent(new GridComponent());
//
//	std::vector<CosmicBodyComponent*> bodies;
//
//	bodies.push_back(new CosmicBodyComponent(3.0f));											//0
//	bodies.push_back(new CosmicBodyComponent(0.7f, 4.0f, 2.0f, &bodies[0]->Position));			//1
//	bodies.push_back(new CosmicBodyComponent(1.0f, 7.0f, 1.0f, &bodies[0]->Position));			//2
//	bodies.push_back(new CosmicBodyComponent(1.0f, 10.0f, 0.8f, &bodies[0]->Position));			//3
//	bodies.push_back(new CosmicBodyComponent(0.3f, 2.0f, 8.0f, &bodies[3]->Position));			//4
//
//	for (auto body : bodies)
//	{
//		MyGame->CreateComponent(body);
//	}
//
//	if (MyGame->Initialize())
//	{
//		MyGame->controller->bodies = bodies;
//		MyGame->Run();
//	}
//
//	MyGame->Exit();
//	delete MyGame;
//
//	return 0;
//}

//Katamari
//int main()
//{
//	Game* MyGame = new Game(L"Katamari");
//
//	MyGame->CreateComponent(new GridComponent());
//
//	auto ball = new KatamariBall();
//	MyGame->CreateComponent(ball);
//
//	//MyGame->CreateComponent(new CosmicBodyComponent(3.0f));
//
//	if (MyGame->Initialize())
//	{
//		MyGame->controller->target = ball;
//		MyGame->Run();
//	}
//
//	MyGame->Exit();
//	delete MyGame;
//
//	return 0;
//}

int main()
{
	Game* MyGame = new Game(L"Katamari");

	//MyGame->CreateComponent(new GridComponent());

	//auto ball = new KatamariBall();
	//MyGame->CreateComponent(ball);

	//MyGame->CreateComponent(new CosmicBodyComponent(3.0f));

	if (MyGame->Initialize())
	{
		auto camera = new GameObject("Camera");
		std::cout << camera->AddComponent(new Camera()) << std::endl;

		auto grid = new GameObject("Grid");
		std::cout << grid->AddComponent(new GridComponent()) << std::endl;


		auto sphere = new GameObject("Sphere");
		auto ball = new KatamariBall();
		std::cout << sphere->AddComponent(ball) << std::endl;

		auto contr = new OrbitCameraController();
		std::cout << camera->AddComponent(contr) << std::endl;

		std::cout << sphere->AddComponent(new KatamariBallController(ball, contr)) << std::endl;

		camera->Transform->SetParent(sphere->Transform);
		camera->Transform->Position = Vector3::Backward * 10.0f + Vector3::Up * 5.0f;

		auto sphere2 = new GameObject("Sphere2");
		std::cout << sphere2->AddComponent(new SphereComponent(0.5f)) << std::endl;
		sphere2->Transform->SetParent(sphere->Transform);
		sphere2->Transform->Position = Vector3::Right * 5.0f + Vector3::Up * 1.0f;


		MyGame->AddGameObject(camera);
		MyGame->AddGameObject(grid);
		MyGame->AddGameObject(sphere);
		MyGame->AddGameObject(sphere2);

		//MyGame->controller->target = ball;
		MyGame->Run();
	}

	MyGame->Exit();
	delete MyGame;

	return 0;
}