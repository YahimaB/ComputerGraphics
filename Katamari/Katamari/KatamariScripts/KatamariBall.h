#pragma once

#include "../Core/Components/RendererComponents/SphereComponent.h"

class KatamariBall : public SphereComponent
{
public:
    BoundingSphere collision{};

    Vector3 oldPos = Vector3::Zero;
    Quaternion textRot = Quaternion::Identity;

public:
    KatamariBall(float radius = 1.0f);
    void Initialize() override;
    void Update(float deltaTime) override;

private:
    float _radius;
};

