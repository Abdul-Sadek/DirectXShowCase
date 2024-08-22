#include "BoxCollider.h"

bool BoxCollider::intersects(const CollisionShape& other) const
{
    return false;
}

DirectX::XMFLOAT3 BoxCollider::getSupportPoint(const DirectX::XMFLOAT3& direction) const
{
    return DirectX::XMFLOAT3();
}
