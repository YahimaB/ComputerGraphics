#pragma once

#include "../MinimalCore.h"
#include "../External/InputDevice.h"

#include "../GameComponent.h"
#include "Camera.h"

using namespace DirectX;
using namespace SimpleMath;

class OrbitCameraController : public GameComponent
{
private:
    Camera* camera;
    InputDevice* inputDevice;

    Quaternion rotation = Quaternion::Identity;
    float sensitivityY = 0.004f;

public:
	OrbitCameraController();

    void Update(float deltaTime) override;
    void OnMouseMove(const InputDevice::MouseMoveEventArgs& args);

    Vector3 GetForward() const;
    Vector3 GetUp() const;
};

