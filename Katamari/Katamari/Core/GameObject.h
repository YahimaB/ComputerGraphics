#pragma once


#include "MinimalCore.h"
#include "GameComponent.h";


class GameObject
{
public:
	GameObject();
	~GameObject() {};

	GameComponent* Transform;

public:
	template <typename T>
	T* GetComponent();

	bool AddComponent(GameComponent* comp);

private:
	std::vector<GameComponent*> _components;
};

