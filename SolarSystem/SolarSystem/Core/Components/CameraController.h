#pragma once
#include "../External/InputDevice.h"

using namespace DirectX;
using namespace SimpleMath;

class Game;

class CameraController
{
public:
    DirectX::SimpleMath::Vector3 up = Vector3::Up;
    DirectX::SimpleMath::Vector3 relativePos;

public:
    Game* game;

    float yaw = 0.0f; //Mouse X || z axis
    float pitch = 0.0f; //Mouse Y || y axis
    float speed = 0.2f;
    float rotationSpeed = 0.005f;

public:
    CameraController();

    void Update();
    void OnMouseMove(const InputDevice::MouseMoveEventArgs& args);
};
