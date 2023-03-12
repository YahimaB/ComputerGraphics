#pragma once

#include "MinimalCore.h"

class Game;

class GameComponent
{
	int _temp;

public:
	GameComponent();
	~GameComponent();

	bool IsUniqueVariant = false;

	DirectX::SimpleMath::Vector3 Position;
	DirectX::SimpleMath::Quaternion Rotation;
	DirectX::SimpleMath::Vector3 Scale = DirectX::SimpleMath::Vector3::One;

public:

	void virtual Initialize() {}; //replace with = 0
	void virtual Draw() {}; //replace with = 0
	void virtual Update(float deltaTime) {}; //replace with = 0
	void virtual DestroyResources() {}; //replace with = 0


	virtual void SetPosition(DirectX::SimpleMath::Vector3 p) { Position = p; }
	virtual void SetRotation(DirectX::SimpleMath::Quaternion q) { Rotation = q; }
	virtual void SetScale(DirectX::SimpleMath::Vector3 s) { Scale = s; }

	virtual DirectX::SimpleMath::Vector3 GetPosition() const { return Position; }
	virtual DirectX::SimpleMath::Quaternion GetRotation() const { return Rotation; }
	virtual DirectX::SimpleMath::Vector3 GetScale() const { return Scale; }

protected:
	Game* game;
};

