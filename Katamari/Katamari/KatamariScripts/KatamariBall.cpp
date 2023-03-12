#include "KatamariBall.h"
#include "../Core/Game.h"
#include "../Core/Components/Transform.h"

using namespace DirectX;
using namespace SimpleMath;

//void KatamariBall::UpdateSize(float absorbedSize)
//{
//    float tmp = sqrtf(gameSize * gameSize + absorbedSize * absorbedSize);
//    //collision.Radius = tmp;
//    Transform->Position.y = tmp;
//    rotationMaxSpeed = 0.1f / (tmp * tmp);
//    if (rotationMaxSpeed < 0.01f)
//        rotationMaxSpeed = 0.01f;
//    moveMaxSpeed = 8.0f * sqrtf(tmp);
//    //outline->UpdateRadius(tmp);
//    gameSize = tmp;
//#ifdef _DEBUG
//    std::cout << tmp << std::endl;
//#endif
//}

KatamariBall::KatamariBall() : SphereComponent(1.0f), gameSize(1.0f)
{
    
}

KatamariBall::~KatamariBall()
{
    //delete outline;
}

void KatamariBall::Initialize()
{
    //outline->Initialize();
    SphereComponent::Initialize();

    collision = BoundingSphere(Transform->Position, 1.0f);
}

//void KatamariBall::Draw()
//{
//    //outline->Draw();
//    SphereComponent::Draw();
//}

void KatamariBall::Update(float deltaTime)
{
    collision.Center = Transform->GetModel().Translation();

    /*for (auto furn : kGame->furniture)
    {
        if (collision.Intersects(furn->collision) && !furn->isPickedUp && gameSize > furn->gameSize)
        {
            furn->isPickedUp = true;
            furn->kb = this;
            furn->initRelPos = furn->GetPosition() - GetPosition();
            rotation.Inverse(furn->invKbRot);
            UpdateSize(furn->gameSize);
        }
    }*/

    auto parentPos = Transform->GetParent()->Position;
    auto posDiff = parentPos - oldPos;
    oldPos = parentPos;

    if (posDiff.Length() > 0.0f)
    {
        float radius = 1.0f;
        float angle = posDiff.Length() / radius;

        Quaternion q = Quaternion::CreateFromAxisAngle(posDiff.Cross(Vector3::Up), -angle);
        Transform->Rotation *= q;
    }

    SphereComponent::Update(deltaTime);
}
