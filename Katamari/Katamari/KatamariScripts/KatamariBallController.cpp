#include "KatamariBallController.h"
#include "../Core/Components/Transform.h"

KatamariBallController::KatamariBallController()
{
    _inputDevice = Game::Instance->InputDevice;

	_inputDevice->MouseMove.AddRaw(this, &KatamariBallController::OnMouseMove);
}

void KatamariBallController::OnMouseMove(const InputDevice::MouseMoveEventArgs& args)
{
	if (_inputDevice->IsKeyDown(Keys::LeftButton))
	{
		Transform->Rotation *= Quaternion::CreateFromAxisAngle(Vector3::Up, -0.004f * args.Offset.x);
	}
}

void KatamariBallController::Update(float deltaTime)
{
    deltaTime /= 1000.0f;
    _velocity *= 1.0f - _moveDrag * deltaTime;

    Vector3 dir = Vector3::Zero;
    if (_inputDevice->IsKeyDown(Keys::W))
        dir += Vector3::Forward;

    if (_inputDevice->IsKeyDown(Keys::S))
        dir += Vector3::Backward;

    if (_inputDevice->IsKeyDown(Keys::D))
        dir += Vector3::Right;

    if (_inputDevice->IsKeyDown(Keys::A))
        dir += Vector3::Left;
    
    if (dir.Length() > 0.0f)
        SetDirection(dir);

    Transform->Position += _velocity * deltaTime;
}

void KatamariBallController::SetDirection(Vector3 dir)
{
    dir = Vector3::Transform(dir, Transform->Rotation);
    dir.Normalize();
    _velocity = dir * _moveMaxSpeed;
}
