#include "KatamariItem.h"
#include "KatamariBall.h"
#include "../Core/Components/Transform.h"

#include "../Core/Game.h"
#include "../Core/GameObject.h"

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

	auto items = Game->FindGameObjects("Item");
	for (auto item : items)
	{
		KatamariItem* kItem = (KatamariItem*)item->GetComponent("KatamariItem");

		if (!kItem->PickedUp && collision.Intersects(kItem->Collision))
		{
			std::cout << "Pick" << std::endl;
			kItem->PickedUp = true;
			kItem->Transform->SetParent(Transform);

			Quaternion inv;
			auto worldRotation = Quaternion::CreateFromRotationMatrix(Transform->GetModel());
			worldRotation.Inverse(inv);

			auto localPos = kItem->Transform->Position - Transform->GetModel().Translation();
			kItem->Transform->Position = Vector3::Transform(localPos, inv);
			worldRotation.Inverse(kItem->Transform->Rotation);
		}
	}

	auto parentPos = Transform->GetParent()->Position;
	auto posDiff = parentPos - oldPos;
	oldPos = parentPos;

	if (posDiff.Length() > 0.0f)
	{
		float radius = 1.0f;
		float angle = posDiff.Length() / radius;

		Quaternion inv;
		Transform->GetParent()->Rotation.Inverse(inv);
		posDiff = Vector3::Transform(posDiff, inv);

		Quaternion q = Quaternion::CreateFromAxisAngle(posDiff.Cross(Vector3::Up), -angle);
		Transform->Rotation *= q;
	}

	SphereComponent::Update(deltaTime);
}
