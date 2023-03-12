#include "KatamariBallController.h"

KatamariBallController::KatamariBallController(KatamariBall* ball, OrbitCameraController* contr)
{
    game = Game::Instance;
	_ball = ball;
	_contr = contr;
}

void KatamariBallController::Update(float deltaTime)
{
    Vector3 dir = Vector3::Zero;
   if (game->InputDevice->IsKeyDown(Keys::W))
       dir += _contr->GetForward();
   if (game->InputDevice->IsKeyDown(Keys::S))
       dir -= _contr->GetForward();
   if (game->InputDevice->IsKeyDown(Keys::A))
       dir -= (_contr->GetForward()).Cross(_contr->GetUp());
   if (game->InputDevice->IsKeyDown(Keys::D))
       dir += (_contr->GetForward()).Cross(_contr->GetUp());
   if (dir.Length() > 0.0f)
       _ball->SetDirection(dir);
}
