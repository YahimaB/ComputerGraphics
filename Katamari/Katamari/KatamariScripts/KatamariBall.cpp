#include "KatamariBall.h"
#include "../Core/Game.h"
#include "../Core/Components/Transform.h"

using namespace DirectX;
using namespace SimpleMath;

void KatamariBall::UpdateSize(float absorbedSize)
{
    float tmp = sqrtf(gameSize * gameSize + absorbedSize * absorbedSize);
    //collision.Radius = tmp;
    Transform->Position.y = tmp;
    rotationMaxSpeed = 0.1f / (tmp * tmp);
    if (rotationMaxSpeed < 0.01f)
        rotationMaxSpeed = 0.01f;
    moveMaxSpeed = 8.0f * sqrtf(tmp);
    //outline->UpdateRadius(tmp);
    gameSize = tmp;
#ifdef _DEBUG
    std::cout << tmp << std::endl;
#endif
}

KatamariBall::KatamariBall() : SphereComponent(1.0f), rotationDrag(0.14f), rotationMaxSpeed(0.1f), moveMaxSpeed(8.0f), moveDrag(5.0f), savedRot(Quaternion::Identity),
velocity(Vector3::Zero), gameSize(1.0f)
{
    
}

KatamariBall::~KatamariBall()
{
    //delete outline;
}

//void KatamariBall::Initialize()
//{
//    //outline->Initialize();
//    SphereComponent::Initialize();
//}

//void KatamariBall::Draw()
//{
//    //outline->Draw();
//    SphereComponent::Draw();
//}

void KatamariBall::Update(float deltaTime)
{
    //collision.Center = Transform->Position;

    deltaTime /= 1000.0f;

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

    savedRot.RotateTowards(Quaternion::Identity, rotationDrag * deltaTime);
    Transform->Rotation *= savedRot;
    velocity *= 1.0f - moveDrag * deltaTime;

    //outline->SetRotation(rotation);

    //outline->Update();
    SphereComponent::Update(deltaTime);

    Transform->Position += velocity * deltaTime;
    //outline->SetPosition(position);
}

//void KatamariBall::Reload()
//{
//    outline->Reload();
//    SphereComponent::Reload();
//}

//void KatamariBall::DestroyResources()
//{
//   //outline->DestroyResources();
//    SphereComponent::DestroyResources();
//}

void KatamariBall::SetDirection(Vector3 dir)
{
    Vector3 tmp = Vector3(dir.x, 0.0f, dir.z);
    tmp.Normalize();
    Quaternion q = Quaternion::CreateFromAxisAngle(tmp.Cross(Vector3::Up), -rotationMaxSpeed);
    float f = Quaternion::Angle(Quaternion::Identity, savedRot) / 0.1f;
    savedRot *= Quaternion::Lerp(q, Quaternion::Identity, f);
    velocity = tmp * moveMaxSpeed;
}

void KatamariBall::SetPosition(Vector3 p)
{
    //outline->SetPosition(p);
    Transform->Position = p;
    //SphereComponent::SetPosition(p);
}
