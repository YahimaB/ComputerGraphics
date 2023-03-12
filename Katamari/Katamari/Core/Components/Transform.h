#pragma once

#include "../MinimalCore.h"
#include "../GameComponent.h"

using namespace DirectX;
using namespace SimpleMath;

class Transform : public GameComponent
{
public:
	Vector3 Position = Vector3::Zero;
	Quaternion Rotation = Quaternion::Identity;
	Vector3 Scale = Vector3::One;
};

