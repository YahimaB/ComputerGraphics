#pragma once

#include "../MinimalCore.h"
#include "../External/InputDevice.h"

#include "../GameComponent.h"
#include "Camera.h"

using namespace DirectX;
using namespace SimpleMath;

class OrbitCameraController : public GameComponent
{
public:
    OrbitCameraController();

    void Update(float deltaTime) override;
    void OnMouseMove(const InputDevice::MouseMoveEventArgs& args);

private:
    Camera* _camera;
    InputDevice* _inputDevice;

    float _sensitivity = 0.005f;
    float _yaw = 0.0f; //Mouse X || around y axis
    float _pitch = 0.0f; //Mouse Y || aroud x axis
};

