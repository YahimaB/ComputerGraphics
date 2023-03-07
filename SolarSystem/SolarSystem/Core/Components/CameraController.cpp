#include "CameraController.h"
#include "../Game.h"

CameraController::CameraController()
{
    game = Game::Instance;

    game->InputDevice->MouseMove.AddRaw(this, &CameraController::OnMouseMove);
    relativePos = game->Camera->Position;
}

void CameraController::OnMouseMove(const InputDevice::MouseMoveEventArgs& args)
{
    if (game->InputDevice->IsKeyDown(Keys::LeftButton))
    {
        yaw -= rotationSpeed * args.Offset.x;
        pitch -= rotationSpeed * args.Offset.y;
    }
}

void CameraController::Update()
{
    if (game->InputDevice->IsKeyDown(Keys::F1))
        game->Camera->Projection = Camera::ProjectionType::Perspective;
    if (game->InputDevice->IsKeyDown(Keys::F2))
        game->Camera->Projection = Camera::ProjectionType::Orthographic;

    Matrix rotationMatrix = Matrix::CreateFromYawPitchRoll(yaw, pitch, 0.0f);
    Vector3 cameraForward = Vector3::Transform(Vector3::Forward, rotationMatrix);
    Vector3 cameraRight = Vector3::Transform(Vector3::Right, rotationMatrix);

    if (game->InputDevice->IsKeyDown(Keys::W))
    {
        game->Camera->Position += speed * cameraForward;
    }
    if (game->InputDevice->IsKeyDown(Keys::S))
    {
        game->Camera->Position -= speed * cameraForward;
    }
    if (game->InputDevice->IsKeyDown(Keys::A))
    {
        game->Camera->Position -= speed * cameraRight;
    }
    if (game->InputDevice->IsKeyDown(Keys::D))
    {
        game->Camera->Position += speed * cameraRight;
    }
    if (game->InputDevice->IsKeyDown(Keys::E))
    {
        game->Camera->Position += speed * Vector3::Up;
    }
    if (game->InputDevice->IsKeyDown(Keys::Z))
    {
        game->Camera->Position -= speed * Vector3::Down;
    }

    game->Camera->Up = Vector3::Transform(Vector3::Up, rotationMatrix);
    game->Camera->Target = game->Camera->Position + cameraForward;
}

