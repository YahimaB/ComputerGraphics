#pragma once

#include "../Core/Game.h"
#include "../Core/GameComponent.h"
#include "../Core/External/InputDevice.h"

using namespace DirectX;
using namespace SimpleMath;

class KatamariBallController : public GameComponent
{
public:
	KatamariBallController();

	void Update(float deltaTime) override;
	void OnMouseMove(const InputDevice::MouseMoveEventArgs& args);

private:
	InputDevice* _inputDevice;

	float _moveMaxSpeed = 8.0f;
	float _moveDrag = 5.0f;
	Vector3 _velocity = Vector3::Zero;

private:

	void SetDirection(Vector3 dir);
};

