#include "GameComponent.h"
#include "GameObject.h"
#include "Components/Transform.h"


void GameComponent::DestroyResources()
{
	this->Transform = nullptr;
	this->GameObject = nullptr;
}

void GameComponent::SetGameObject(::GameObject* gameObject)
{
	this->GameObject = gameObject;
	this->Transform = gameObject->Transform;
}
