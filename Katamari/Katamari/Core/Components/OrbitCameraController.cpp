#include "OrbitCameraController.h"
#include "../Game.h"
#include "Transform.h"

OrbitCameraController::OrbitCameraController(GameComponent* t)
{
	camera = Camera::Current;
	inputDevice = Game::Instance->InputDevice;

	target = t;

	inputDevice->MouseMove.AddRaw(this, &OrbitCameraController::OnMouseMove);
}

void OrbitCameraController::OnMouseMove(const InputDevice::MouseMoveEventArgs& args)
{
	if (inputDevice->IsKeyDown(Keys::LeftButton) || !isLMBActivated)
	{
		Vector3 tmp = Vector3::Transform(Vector3::Right, rotation);
		if ((GetForward().y < 0 || inputDevice->MouseOffset.y < 0) && (GetUp().y > 0.05f || inputDevice->MouseOffset.y > 0))
		{
			rotation *= Quaternion::CreateFromAxisAngle(tmp, sensitivityY * inputDevice->MouseOffset.y);
		}

		rotation *= Quaternion::CreateFromAxisAngle(Vector3::Up, -sensitivityX * inputDevice->MouseOffset.x);
	}

	if ((radius > 5.0 || inputDevice->MouseWheelDelta < 0) && (radius < 30.0 || inputDevice->MouseWheelDelta > 0))
	{
		radius -= 0.01f * inputDevice->MouseWheelDelta;
	}
}

void OrbitCameraController::Update(float deltaTime)
{
	camera->Transform->Position = target->Transform->Position - Vector3::Forward * radius;


	//camera->Transform->Position = target->Transform->Position - GetForward() * radius;
	//camera->Transform->Rotation = rotation;
	target->Transform->Rotation = rotation;
}

Vector3 OrbitCameraController::GetForward() const
{
	return Vector3::Transform(Vector3::Forward, rotation);
}

Vector3 OrbitCameraController::GetUp() const
{
	return Vector3::Transform(Vector3::Up, rotation);
}