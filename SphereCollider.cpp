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
bool SphereCollider::intersectsBox(const CollisionShape& box) const
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
bool SphereCollider::intersectsPlane(const CollisionShape& plane) const
{
    const PlaneCollider* othershape = dynamic_cast<const PlaneCollider*>(&plane);
    // Load the plane normal and sphere center into XMVECTORs
    DirectX::XMVECTOR normal = DirectX::XMLoadFloat3(&othershape->normal);
    DirectX::XMVECTOR sphereCenter = DirectX::XMLoadFloat3(&this->center);

    // Calculate the distance from the sphere center to the plane
    float distance = DirectX::XMVectorGetX(DirectX::XMVector3Dot(normal, sphereCenter)) + othershape->d;
    distance = fabs(distance); // Absolute value of distance

    // Check if the distance is less than or equal to the sphere's radius
    return distance <= this->radius;
}

bool SphereCollider::intersectsSphere(const CollisionShape& sphere) const
{
    const SphereCollider* othershape = dynamic_cast<const SphereCollider*>(&sphere);
    // Load the sphere centers into XMVECTORs
    DirectX::XMVECTOR centerA = DirectX::XMLoadFloat3(&this->center);
    DirectX::XMVECTOR centerB = DirectX::XMLoadFloat3(&othershape->center);

    // Calculate the squared distance between the centers
    DirectX::XMVECTOR delta = DirectX::XMVectorSubtract(centerA, centerB);
    float distanceSquared = DirectX::XMVectorGetX(DirectX::XMVector3LengthSq(delta));

    // Calculate the sum of the radii
    float radiiSum = this->radius + othershape->radius;

    // Check if the squared distance is less than or equal to the squared radii sum
    return distanceSquared <= (radiiSum * radiiSum);
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
