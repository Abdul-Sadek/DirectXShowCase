#include "SphereCollider.h"

// Check if this sphere intersects with another shape
bool SphereCollider::intersects(const CollisionShape& other) const
{
    // Assume we're only handling sphere-sphere collision for now
    const SphereCollider* otherSphere = dynamic_cast<const SphereCollider*>(&other);
    if (otherSphere)
    {
        DirectX::XMFLOAT3 centerA = getCenter();
        DirectX::XMFLOAT3 centerB = otherSphere->getCenter();
        DirectX::XMFLOAT3 delta;
        delta.x = centerB.x - centerA.x;
        delta.y = centerB.y - centerA.y;
        delta.z = centerB.z - centerA.z;

        float distanceSq = delta.x * delta.x + delta.y * delta.y + delta.z * delta.z;
        float radiusSum = radius + otherSphere->radius;

        return distanceSq <= (radiusSum * radiusSum);
    }
    return false;
}

// Get the farthest point on the sphere in the given direction
DirectX::XMFLOAT3 SphereCollider::getSupportPoint(const DirectX::XMFLOAT3& direction) const
{
    DirectX::XMFLOAT3 normalizedDirection = direction;
    DirectX::XMStoreFloat3(&normalizedDirection, DirectX::XMVector3Normalize(DirectX::XMLoadFloat3(&direction)));

    DirectX::XMFLOAT3 supportPoint;
    supportPoint.x = getCenter().x + normalizedDirection.x * radius;
    supportPoint.y = getCenter().y + normalizedDirection.y * radius;
    supportPoint.z = getCenter().z + normalizedDirection.z * radius;

    return supportPoint;
}

// Update the transform matrix
void SphereCollider::updateTransform(const DirectX::XMFLOAT4X4& newTransform)
{
    transform = newTransform;
}

// Get the center of the sphere in world space
DirectX::XMFLOAT3 SphereCollider::getCenter() const
{
    return DirectX::XMFLOAT3(transform._41, transform._42, transform._43); // Translation part of the matrix
}

// Get the bounding radius of the sphere
float SphereCollider::getBoundingRadius() const
{
    return radius;
}
