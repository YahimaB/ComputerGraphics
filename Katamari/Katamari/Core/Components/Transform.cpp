#include "Transform.h"

Transform* Transform::GetParent()
{
    return _parent;
}

void Transform::SetParent(Transform* parent)
{
    this->_parent = parent;
}

Matrix Transform::GetLocalModel()
{
    return Matrix::CreateScale(Scale) * Matrix::CreateFromQuaternion(Rotation) * Matrix::CreateTranslation(Position);
}

Matrix Transform::GetModel()
{
    if (_parent == nullptr)
    {
        return GetLocalModel();
    }

    return GetLocalModel() * _parent->GetModel();
}
