#pragma once

#include "MinimalCore.h"

class Transform;
class GameObject;

class GameComponent
{
public:
	Transform* Transform = nullptr;
	GameObject* GameObject = nullptr;

public:

	void virtual Initialize() {};
	void virtual Draw() {};
	void virtual Update(float deltaTime) {};
	void virtual DestroyResources();

protected:
	friend class GameObject;

	void SetGameObject(::GameObject* gameObject);
	std::string virtual GetUniqueId() { return std::string(); }
};

#include "Components/Transform.h"

