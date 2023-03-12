#pragma once

#include "../Core/GameComponent.h"
#include "../Core/Components/OrbitCameraController.h"
#include "KatamariBall.h"

class KatamariBallController : public GameComponent
{
public:
	KatamariBallController(KatamariBall* ball, OrbitCameraController* contr);

	void Update(float deltaTime) override;

private:
	Game* game;
	InputDevice* inputDevice;


	KatamariBall* _ball;
	OrbitCameraController* _contr;

	void OnMouseMove(const InputDevice::MouseMoveEventArgs& args);
};

