#include "OrbitCameraController.h"
#include "../Game.h"
#include "Transform.h"
//#include "../../KatamariScripts/KatamariBall.h"

OrbitCameraController::OrbitCameraController(GameComponent* t)
{
    camera = Camera::Current;
    game = Game::Instance;

    target = t;

    game->InputDevice->MouseMove.AddRaw(this, &OrbitCameraController::OnMouseMove);

    //target = t;
}

void OrbitCameraController::OnMouseMove(const InputDevice::MouseMoveEventArgs& args)
{
    if (game->InputDevice->IsKeyDown(Keys::LeftButton) || !isLMBActivated)
    {
        //bool temp = target != nullptr;
        //std::cout << temp << std::endl;

        Vector3 tmp = Vector3::Transform(Vector3::Right, rotation);
        if ((GetForward().y < 0 || game->InputDevice->MouseOffset.y < 0) && (GetUp().y > 0.05f || game->InputDevice->MouseOffset.y > 0))
            rotation *= Quaternion::CreateFromAxisAngle(tmp, sensitivityY * game->InputDevice->MouseOffset.y);
        rotation *= Quaternion::CreateFromAxisAngle(Vector3::Up, -sensitivityX * game->InputDevice->MouseOffset.x);
    }
    if ((radius > 5.0 || game->InputDevice->MouseWheelDelta < 0) && (radius < 30.0 || game->InputDevice->MouseWheelDelta > 0))
        radius -= 0.01f * game->InputDevice->MouseWheelDelta;
}

void OrbitCameraController::Update(float deltaTime)
{


    camera->Position = target->Transform->Position - GetForward() * radius;
    camera->Up = Vector3::Transform(Vector3::Up, rotation);
    camera->Target = target->Transform->Position;

    //std::cout << camera->Target.x << " : " << camera->Target.y << std::endl;


    /*Vector3 dir = Vector3::Zero;
    if (game->InputDevice->IsKeyDown(Keys::W))
        dir += GetForward();
    if (game->InputDevice->IsKeyDown(Keys::S))
        dir -= GetForward();
    if (game->InputDevice->IsKeyDown(Keys::A))
        dir -= (GetForward()).Cross(GetUp());
    if (game->InputDevice->IsKeyDown(Keys::D))
        dir += (GetForward()).Cross(GetUp());
    if (dir.Length() > 0.0f)
        target->SetDirection(dir);*/
}

Vector3 OrbitCameraController::GetForward() const
{
    return Vector3::Transform(Vector3::Forward, rotation);
}

Vector3 OrbitCameraController::GetUp() const
{
    return Vector3::Transform(Vector3::Up, rotation);
}