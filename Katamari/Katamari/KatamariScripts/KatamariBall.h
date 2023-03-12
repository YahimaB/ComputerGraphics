#pragma once

#include "../Core/Components/RendererComponents/SphereComponent.h"

class KatamariBall : public SphereComponent
{
public:
    Vector3 oldPos = Vector3::Zero;
    Quaternion textRot = Quaternion::Identity;

    BoundingSphere collision{};

    //KatamariBallOutline* outline;

    


    /*float moveMaxSpeed;
    float moveDrag;
    DirectX::SimpleMath::Vector3 velocity;*/
    float gameSize;

public:
    KatamariBall();
    ~KatamariBall();
    void Initialize() override;
    //void Draw() override;
    void Update(float deltaTime) override;
    //void Reload() override;
    //void DestroyResources() override;

protected:
    //void UpdateSize(float absorbedSize);
};

