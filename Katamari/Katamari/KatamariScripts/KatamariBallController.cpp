#include "KatamariBallController.h"
#include "../Core/Components/Transform.h"

KatamariBallController::KatamariBallController(KatamariBall* ball, OrbitCameraController* contr)
{
    game = Game::Instance;
	_ball = ball;
	_contr = contr;

    inputDevice = Game::Instance->InputDevice;
	inputDevice->MouseMove.AddRaw(this, &KatamariBallController::OnMouseMove);
}

void KatamariBallController::OnMouseMove(const InputDevice::MouseMoveEventArgs& args)
{
	if (inputDevice->IsKeyDown(Keys::LeftButton))
	{
		_ball->Transform->Rotation *= Quaternion::CreateFromAxisAngle(Vector3::Up, -0.004f * inputDevice->MouseOffset.x);
	}
}

void KatamariBallController::Update(float deltaTime)
{
    Vector3 dir = Vector3::Zero;
   if (game->InputDevice->IsKeyDown(Keys::W))
       dir += _ball->GetForward();
   if (game->InputDevice->IsKeyDown(Keys::S))
       dir -= _ball->GetForward();
   if (game->InputDevice->IsKeyDown(Keys::A))
       dir -= (_ball->GetForward()).Cross(_ball->GetUp());
   if (game->InputDevice->IsKeyDown(Keys::D))
       dir += (_ball->GetForward()).Cross(_ball->GetUp());
   if (dir.Length() > 0.0f)
       _ball->SetDirection(dir);
}
