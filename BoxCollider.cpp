#include "BoxCollider.h"
#include <utility>

BoxCollider::BoxCollider(const DirectX::XMFLOAT3& boxExtents)
    : extents(boxExtents) {}


// Check if this box intersects with another shape
bool BoxCollider::intersects(const CollisionShape& other) const
{
    return other.intersectsBox(*this);
}

bool BoxCollider::intersectsBox(const CollisionShape& box) const
{
    const BoxCollider* othershape = dynamic_cast<const BoxCollider*>(&box);
    if (othershape != nullptr)
    {
        DirectX::XMFLOAT3 centerA = getCenter();
        DirectX::XMFLOAT3 centerB = othershape->getCenter();

        // Check overlap along each axis
        if (fabs(centerA.x - centerB.x) > (extents.x + othershape->extents.x)) return false;
        if (fabs(centerA.y - centerB.y) > (extents.y + othershape->extents.y)) return false;
        if (fabs(centerA.z - centerB.z) > (extents.z + othershape->extents.z)) return false;

        return true; // If we pass all axis checks, the boxes intersect
    }

    return false; // By default, return false for other shape types
}
bool BoxCollider::intersectsPlane(const CollisionShape& plane) const
{
    const PlaneCollider* othershape = dynamic_cast<const PlaneCollider*>(&plane);
    // Calculate the box's center point
    DirectX::XMFLOAT3 centerA = getCenter();

    // Calculate the distance from the box's center point to the plane
    float distance = othershape->normal.x * centerA.x + othershape->normal.y * centerA.y + othershape->normal.z * centerA.z - othershape->d;

    // Check if the box's center point lies on the same side of the plane as the box's vertices
    if (distance >= 0.0f)
    {
        // Check if any of the box's vertices lie on the opposite side of the plane
        if (othershape->normal.x * extents.x + othershape->normal.y * extents.y + othershape->normal.z * extents.z - othershape->d < 0.0f ||
            othershape->normal.x * extents.x + othershape->normal.y * extents.y + othershape->normal.z * extents.z - othershape->d < 0.0f)
        {
            return true; // Box intersects with plane
        }
    }
    return false; // Box does not intersect with plane
}

bool BoxCollider::intersectsSphere(const CollisionShape& sphere) const
{
    const SphereCollider* othershape = dynamic_cast<const SphereCollider*>(&sphere);
    // Calculate the closest point on the box to the sphere's center point
    float closestX = std::max(extents.x, std::min(othershape->center.x, extents.x));
    float closestY = std::max(extents.y, std::min(othershape->center.y, extents.y));
    float closestZ = std::max(extents.z, std::min(othershape->center.z, extents.z));

    // Calculate the distance from the sphere's center point to the closest point on the box
    float distance = sqrt((othershape->center.x - closestX) * (othershape->center.x - closestX) +
        (othershape->center.y - closestY) * (othershape->center.y - closestY) +
        (othershape->center.z - closestZ) * (othershape->center.z - closestZ));

    // Check if the distance is less than or equal to the sphere's radius
    if (distance <= othershape->radius)
    {
        return true; // Box intersects with sphere
    }
    return false; // Box does not intersect with sphere
}

// Get the farthest point on the box in the given direction
DirectX::XMFLOAT3 BoxCollider::getSupportPoint(const DirectX::XMFLOAT3& direction) const
{
    DirectX::XMFLOAT3 supportPoint = getCenter();

    supportPoint.x += (direction.x >= 0 ? extents.x : -extents.x);
    supportPoint.y += (direction.y >= 0 ? extents.y : -extents.y);
    supportPoint.z += (direction.z >= 0 ? extents.z : -extents.z);

    return supportPoint;
}

// Update the transform matrix
void BoxCollider::updateTransform(const DirectX::XMFLOAT4X4& newTransform)
{
    transform = newTransform;
}

// Get the center of the box in world space
DirectX::XMFLOAT3 BoxCollider::getCenter() const
{
    return DirectX::XMFLOAT3(transform._41, transform._42, transform._43); // Translation part of the matrix
}

// Get the bounding radius of the box
float BoxCollider::getBoundingRadius() const
{
    // Compute the bounding radius (distance from center to a corner of the box)
    return sqrtf(extents.x * extents.x + extents.y * extents.y + extents.z * extents.z);
}
