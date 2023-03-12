#include "GameObject.h"
#include <typeinfo>


GameObject::GameObject(std::string name)
{
	this->Name = name;

	Transform = new ::Transform();
	AddComponent(Transform);
}

void GameObject::Update(float deltaTime)
{
	//std::cout << "Updating GO: " << Name << " with " << _components.size() << " components" << std::endl;

	for (auto component : _components)
	{
		component->Update(deltaTime);
	}
}

void GameObject::Draw()
{
	for (auto component : _components)
	{
		component->Draw();
	}
}

void GameObject::DestroyResources()
{
	for (auto component : _components)
	{
		component->DestroyResources();
	}

	_components.clear();
	_components.shrink_to_fit();
}


template <typename T>
T* GameObject::GetComponent()
{
	const type_info& typeInfo = typeid(T);
	for (auto component : _components) {
		if (typeid(component) == typeInfo)
		{
			return component;
		}
	}

	return nullptr;
}

bool GameObject::AddComponent(GameComponent *comp)
{
	auto uniqueId = comp->GetUniqueId();
	if (uniqueId.compare(std::string()))
	{
		/*const type_info &typeInfo = typeid(comp);
		for (auto component : _components) {
			if (typeid(component) == typeInfo)
			{
				std::cout << typeid(component).name() << " == " << typeInfo.name() << std::endl;
				return false;
			}
		}*/
		for (auto component : _components) {
			if (!uniqueId.compare(component->GetUniqueId()))
			{
				return false;
			}
		}
	}

	comp->SetGameObject(this);
	_components.push_back(comp);

	comp->Initialize();
	return true;
}
