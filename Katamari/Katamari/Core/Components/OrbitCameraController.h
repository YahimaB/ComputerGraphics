#pragma once

#include "../MinimalCore.h"
#include "../Game.h"
#include "../External/InputDevice.h"

#include "../GameComponent.h"
#include "Camera.h"

using namespace DirectX;
using namespace SimpleMath;

//class KatamariBall;

class OrbitCameraController : public GameComponent
{
private:
    Game* game;
    Camera* camera;
    //InputDevice* inputDevice;
    //GameComponent* target;



    Quaternion rotation = Quaternion::Identity;
    float radius = 10.0f;
    float sensitivityX = 0.004f;
    float sensitivityY = 0.004f;
    bool isLMBActivated = true;

public:
    GameComponent* target;

	OrbitCameraController(GameComponent* t);

    void Update(float deltaTime) override;

    void OnMouseMove(const InputDevice::MouseMoveEventArgs& args);
    Vector3 GetForward() const;
    Vector3 GetUp() const;
    void SetSensitivity(float sX, float sY) { sensitivityX = sX; sensitivityY = sY; }
    void SetSensitivityX(float sX) { sensitivityX = sX; }
    void SetSensitivityY(float sY) { sensitivityY = sY; }
    float GetSensitivityX() const { return sensitivityX; }
    float GetSensitivityY() const { return sensitivityX; }
};

