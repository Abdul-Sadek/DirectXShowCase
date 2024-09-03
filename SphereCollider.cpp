#include "SphereCollider.h"
#include <utility>

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
bool SphereCollider::intersectsBox(const BoxCollider& box) const
{
    const BoxCollider* othershape = dynamic_cast<const BoxCollider*>(&box);
    // Calculate the box's center point
    DirectX::XMFLOAT3 centerA = othershape->getCenter();
    // Calculate the closest point on the box to the sphere's center point
    float closestX = std::max(center.x + radius, std::min(centerA.x, center.x + radius));
    float closestY = std::max(center.y + radius, std::min(centerA.y, center.y + radius));
    float closestZ = std::max(center.z + radius, std::min(centerA.z, center.z + radius));

    // Calculate the distance from the sphere's center point to the closest point on the box
    float distance = sqrt((centerA.x - closestX) * (centerA.x - closestX) +
        (centerA.y - closestY) * (centerA.y - closestY) +
        (centerA.z - closestZ) * (centerA.z - closestZ));

    // Check if the distance is less than or equal to the sphere's radius
    if (distance <= radius)
    {
        return true; // Box intersects with sphere
    }
    return false; // Box does not intersect with sphere
}
bool SphereCollider::intersectsPlane(const PlaneCollider& plane) const
{
    // Handle box-plane intersection
    return false;
}

bool SphereCollider::intersectsSphere(const SphereCollider& sphere) const
{
    // Handle box-sphere intersection
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
