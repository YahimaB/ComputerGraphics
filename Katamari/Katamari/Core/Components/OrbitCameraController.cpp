#include "OrbitCameraController.h"
#include "../Game.h"
#include "Transform.h"

OrbitCameraController::OrbitCameraController()
{
	_camera = Camera::Current;
	_inputDevice = Game::Instance->InputDevice;

	_inputDevice->MouseMove.AddRaw(this, &OrbitCameraController::OnMouseMove);
}

void OrbitCameraController::OnMouseMove(const InputDevice::MouseMoveEventArgs& args)
{
	if (_inputDevice->IsKeyDown(Keys::LeftButton))
	{
		float xValue = _sensitivity * args.Offset.x;
		float yValue = _sensitivity * args.Offset.y;

		_yaw -= xValue;
		_pitch -= yValue;
	}
}

void OrbitCameraController::Update(float deltaTime)
{
	//std::cout << _pitch << std::endl;
	Quaternion rotationMatrix = Quaternion::CreateFromYawPitchRoll(0.0f, _pitch, 0.0f);

	_camera->Transform->Rotation = rotationMatrix;
}