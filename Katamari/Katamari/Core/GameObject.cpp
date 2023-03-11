#include "GameObject.h"
#include <typeinfo>


GameObject::GameObject()
{
	_components.clear();

	Transform = new GameComponent();
	AddComponent(Transform);
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
	if (comp->IsUniqueVariant)
	{
		const type_info &typeInfo = typeid(comp);
		for (auto component : _components) {
			if (typeid(component) == typeInfo)
			{
				return false;
			}
		}
	}

	_components.push_back(comp);
	return true;
}
