#include "OrbitCameraController.h"
#include "../Game.h"
#include "Transform.h"

OrbitCameraController::OrbitCameraController()
{
	camera = Camera::Current;
	inputDevice = Game::Instance->InputDevice;

	inputDevice->MouseMove.AddRaw(this, &OrbitCameraController::OnMouseMove);
}

void OrbitCameraController::OnMouseMove(const InputDevice::MouseMoveEventArgs& args)
{
	if (inputDevice->IsKeyDown(Keys::LeftButton))
	{
		Vector3 tmp = Vector3::Transform(Vector3::Right, rotation);
		if ((GetForward().y < 0 || inputDevice->MouseOffset.y > 0) && (GetUp().y > 0.05f || inputDevice->MouseOffset.y < 0))
		{
			rotation *= Quaternion::CreateFromAxisAngle(tmp, -sensitivityY * inputDevice->MouseOffset.y);
		}
	}
}

void OrbitCameraController::Update(float deltaTime)
{
	camera->Transform->Rotation = rotation;
}

Vector3 OrbitCameraController::GetForward() const
{
	return Vector3::Transform(Vector3::Forward, rotation);
}

Vector3 OrbitCameraController::GetUp() const
{
	return Vector3::Transform(Vector3::Up, rotation);
}