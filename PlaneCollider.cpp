#include "PlaneCollider.h"

bool PlaneCollider::intersects(const CollisionShape& other) const
{
    return false;
}

DirectX::XMFLOAT3 PlaneCollider::getSupportPoint(const DirectX::XMFLOAT3& direction) const
{
    return DirectX::XMFLOAT3();
}

void PlaneCollider::updateTransform(const DirectX::XMFLOAT4X4& newTransform)
{
}

DirectX::XMFLOAT3 PlaneCollider::getCenter() const
{
    return DirectX::XMFLOAT3();
}

float PlaneCollider::getBoundingRadius() const
{
    return 0.0f;
}
