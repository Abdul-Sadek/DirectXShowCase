#include "BoxCollider.h"

BoxCollider::BoxCollider(const DirectX::XMFLOAT3& boxExtents)
    : extents(boxExtents) {}

// Check if this box intersects with another shape
bool BoxCollider::intersects(const CollisionShape& other) const
{
    // Handle box-box intersection for now
    const BoxCollider* otherBox = dynamic_cast<const BoxCollider*>(&other);
    if (otherBox)
    {
        DirectX::XMFLOAT3 centerA = getCenter();
        DirectX::XMFLOAT3 centerB = otherBox->getCenter();

        // Check overlap along each axis
        if (fabs(centerA.x - centerB.x) > (extents.x + otherBox->extents.x)) return false;
        if (fabs(centerA.y - centerB.y) > (extents.y + otherBox->extents.y)) return false;
        if (fabs(centerA.z - centerB.z) > (extents.z + otherBox->extents.z)) return false;

        return true; // If we pass all axis checks, the boxes intersect
    }

    return false; // By default, return false for other shape types
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
