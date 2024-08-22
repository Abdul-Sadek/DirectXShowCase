#include "SphereCollider.h"

bool SphereCollider::intersects(const CollisionShape& other) const
{
    return false;
}

DirectX::XMFLOAT3 SphereCollider::getSupportPoint(const DirectX::XMFLOAT3& direction) const
{
    return DirectX::XMFLOAT3();
}
