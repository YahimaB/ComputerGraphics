#pragma once

#include "../MinimalCore.h"

#include "Camera.h"
//#include "../GameComponent.h"
#include "../External/InputDevice.h"
//#include "../../KatamariScripts/KatamariBall.h"

using namespace DirectX;
using namespace SimpleMath;

class KatamariBall;

class OrbitCameraController
{
private:
    Game* game;
    Camera* camera;
    //GameComponent* target;



    Quaternion rotation = Quaternion::Identity;
    float radius = 10.0f;
    float sensitivityX = 0.004f;
    float sensitivityY = 0.004f;
    bool isLMBActivated = true;

public:
    KatamariBall* target;

	OrbitCameraController();

    void OnMouseMove(const InputDevice::MouseMoveEventArgs& args);
    void Update();
    Vector3 GetForward() const;
    Vector3 GetUp() const;
    void SetSensitivity(float sX, float sY) { sensitivityX = sX; sensitivityY = sY; }
    void SetSensitivityX(float sX) { sensitivityX = sX; }
    void SetSensitivityY(float sY) { sensitivityY = sY; }
    float GetSensitivityX() const { return sensitivityX; }
    float GetSensitivityY() const { return sensitivityX; }
};

