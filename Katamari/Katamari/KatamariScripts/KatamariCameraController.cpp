#include "KatamariCameraController.h"

#include "../Core/Game.h"
#include "../Core/Components/Transform.h"

KatamariCameraController::KatamariCameraController()
{
	_camera = Camera::Current;
	_inputDevice = Game::Instance->InputDevice;

	_inputDevice->MouseMove.AddRaw(this, &KatamariCameraController::OnMouseMove);
}

void KatamariCameraController::OnMouseMove(const InputDevice::MouseMoveEventArgs& args)
{
	if (_inputDevice->IsKeyDown(Keys::LeftButton))
	{
		float xValue = _sensitivity * args.Offset.x;
		float yValue = _sensitivity * args.Offset.y;

		_yaw -= xValue;
		_pitch -= yValue;
	}
}

void KatamariCameraController::Update(float deltaTime)
{
	//std::cout << _pitch << std::endl;
	Quaternion rotationMatrix = Quaternion::CreateFromYawPitchRoll(0.0f, _pitch, 0.0f);

	_camera->Transform->Rotation = rotationMatrix;
}