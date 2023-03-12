#pragma once

#include "../Core/MinimalCore.h"
#include "../Core/External/InputDevice.h"

#include "../Core/GameComponent.h"
#include "../Core/Components/Camera.h"

using namespace DirectX;
using namespace SimpleMath;

class KatamariCameraController : public GameComponent
{
public:
    KatamariCameraController();

    void Update(float deltaTime) override;
    void OnMouseMove(const InputDevice::MouseMoveEventArgs& args);

private:
    Camera* _camera;
    InputDevice* _inputDevice;

    float _sensitivity = 0.005f;
    float _yaw = 0.0f; //Mouse X || around y axis
    float _pitch = 0.0f; //Mouse Y || aroud x axis
};

