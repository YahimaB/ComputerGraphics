//#include "CameraController.h"
//#include "../../ShapeComponents/CosmicBodyComponent.h"
//#include "../Game.h"
//
//CameraController::CameraController()
//{
//    game = Game::Instance;
//
//    game->InputDevice->MouseMove.AddRaw(this, &CameraController::OnMouseMove);
//    ChildPosition = game->Camera->Position;
//}
//
//void CameraController::OnMouseMove(const InputDevice::MouseMoveEventArgs& args)
//{
//    if (game->InputDevice->IsKeyDown(Keys::LeftButton))
//    {
//        float xValue = rotationSpeed * args.Offset.x;
//        float yValue = rotationSpeed * args.Offset.y;
//
//        if (targetOrbitPos == nullptr)
//        {
//            yaw -= xValue;
//            pitch -= yValue;
//        }
//        else
//        {
//            yaw = xValue;
//            pitch = yValue;
//        }
//    }
//
//    if (args.WheelDelta != 0)
//    {
//        if (game->Camera->Projection == Camera::ProjectionType::Perspective)
//        {
//            game->Camera->FOV += fovSpeed * args.WheelDelta;
//        }
//        else
//        {
//            game->Camera->OrthoSize += 0.01f * args.WheelDelta;
//        }
//    }
//}
//
//void CameraController::Update()
//{
//    if (game->InputDevice->IsKeyDown(Keys::F1))
//    {
//        game->Camera->Projection = Camera::ProjectionType::Perspective;
//    }
//    if (game->InputDevice->IsKeyDown(Keys::F2))
//    {
//        game->Camera->Projection = Camera::ProjectionType::Orthographic;
//    }
//
//    int zeroKeyCode = 48;
//    for (int i = 0; i < bodies.size(); i++)
//    {
//        int keyCode = zeroKeyCode + i;
//
//        if (game->InputDevice->IsKeyDown(static_cast<Keys>(keyCode)))
//        {
//            targetOrbitPos = &bodies[i]->Position;
//
//            game->Camera->Up = Vector3::Up;
//            game->Camera->Position = Vector3::Forward * 20.0f;
//
//            yaw = 0.0f;
//            pitch = 0.0f;
//        }
//    }
//
//    if (game->InputDevice->IsKeyDown(Keys::Tab))
//    {
//        targetOrbitPos = nullptr;
//
//        auto q = Quaternion::LookRotation(game->Camera->Target - game->Camera->Position, game->Camera->Up);
//        auto v = q.ToEuler();
//
//        yaw = v.y;
//        pitch = 0.0f;
//    }
//
//
//    if (targetOrbitPos == nullptr)
//    {
//        Matrix rotationMatrix = Matrix::CreateFromYawPitchRoll(yaw, pitch, 0.0f);
//        Vector3 cameraForward = Vector3::Transform(Vector3::Forward, rotationMatrix);
//        Vector3 cameraRight = Vector3::Transform(Vector3::Right, rotationMatrix);
//
//        if (game->InputDevice->IsKeyDown(Keys::W))
//        {
//            game->Camera->Position += speed * cameraForward;
//        }
//        if (game->InputDevice->IsKeyDown(Keys::S))
//        {
//            game->Camera->Position -= speed * cameraForward;
//        }
//        if (game->InputDevice->IsKeyDown(Keys::A))
//        {
//            game->Camera->Position -= speed * cameraRight;
//        }
//        if (game->InputDevice->IsKeyDown(Keys::D))
//        {
//            game->Camera->Position += speed * cameraRight;
//        }
//        if (game->InputDevice->IsKeyDown(Keys::E))
//        {
//            game->Camera->Position += speed * Vector3::Up;
//        }
//        if (game->InputDevice->IsKeyDown(Keys::Z))
//        {
//            game->Camera->Position -= speed * Vector3::Down;
//        }
//
//        game->Camera->Up = Vector3::Transform(Vector3::Up, rotationMatrix);
//        game->Camera->Target = game->Camera->Position + cameraForward;
//    }
//    else
//    {
//        Vector3 up = game->Camera->Up;
//        Vector3 right = ChildPosition.Cross(up);
//        Quaternion quat = Quaternion::CreateFromAxisAngle(up, yaw) * Quaternion::CreateFromAxisAngle(right, pitch);
//
//        ChildPosition = Vector3::Transform(ChildPosition, Matrix::CreateFromQuaternion(quat));
//
//        game->Camera->Up = Vector3::Transform(up, Matrix::CreateFromQuaternion(quat));;
//
//        game->Camera->Target = *targetOrbitPos;
//        game->Camera->Position = *targetOrbitPos + ChildPosition;
//
//        yaw = 0.0f;
//        pitch = 0.0f;
//    }
//}
//
