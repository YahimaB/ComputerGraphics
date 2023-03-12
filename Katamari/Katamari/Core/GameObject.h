#pragma once


#include "MinimalCore.h"
#include "GameComponent.h";
#include "Components/Transform.h"


class GameObject
{
public:
	std::string Name = "none";
	Transform* Transform = nullptr;

public:
	GameObject(std::string name);
	~GameObject() {};

	void virtual Update(float deltaTime);
	void virtual Draw();
	void virtual DestroyResources();

	bool AddComponent(GameComponent* comp);
	GameComponent* GetComponent(std::string id);

	//template <typename T> T* GetComponent();


private:
	std::vector<GameComponent*> _components;
};

