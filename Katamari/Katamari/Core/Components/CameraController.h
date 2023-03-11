#pragma once
#include "../External/InputDevice.h"

using namespace DirectX;
using namespace SimpleMath;

class Game;
class CosmicBodyComponent;

class CameraController
{
public:
    Vector3 ChildPosition;

public:
    Game* game;

    float rotationSpeed = 0.005f;
    float fovSpeed = 0.001f;

    std::vector<CosmicBodyComponent*> bodies;
    Vector3* targetOrbitPos = nullptr;

    float yaw = 0.0f; //Mouse X || around y axis
    float pitch = 0.0f; //Mouse Y || aroud x axis
    float speed = 0.2f;

public:
    CameraController();

    void Update();
    void OnMouseMove(const InputDevice::MouseMoveEventArgs& args);
};
