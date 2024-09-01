#include "PlaneCollider.h"
#include "SphereCollider.h"

// Constructor
PlaneCollider::PlaneCollider(const DirectX::XMFLOAT3& normal, float d)
    : normal(normal), d(d)
{
    // Normalize the normal vector
    DirectX::XMStoreFloat3(&this->normal, DirectX::XMVector3Normalize(DirectX::XMLoadFloat3(&normal)));
}

// Check if this plane intersects with another shape
bool PlaneCollider::intersects(const CollisionShape& other) const
{
    // Assume the basic case where we're checking for a sphere-plane intersection
    const SphereCollider* sphere = dynamic_cast<const SphereCollider*>(&other);
    if (sphere)
    {
        // Store the sphere center in a local variable
        DirectX::XMFLOAT3 sphereCenterFloat3 = sphere->getCenter();
        // Load the sphere center into an XMVECTOR
        DirectX::XMVECTOR sphereCenter = DirectX::XMLoadFloat3(&sphereCenterFloat3);
        // Calculate the distance from the sphere's center to the plane
        DirectX::XMVECTOR planeNormal = DirectX::XMLoadFloat3(&normal);
        float distance = DirectX::XMVectorGetX(DirectX::XMVector3Dot(planeNormal, sphereCenter)) - d;

        // Check if the distance is less than or equal to the sphere's radius
        return fabs(distance) <= sphere->getBoundingRadius();
    }

    // Additional checks for other types of CollisionShapes can be added here
    return false;
}

// Get the farthest point on the plane in the given direction
DirectX::XMFLOAT3 PlaneCollider::getSupportPoint(const DirectX::XMFLOAT3& direction) const
{
    // Since a plane is infinite, the support point in a direction is a bit ambiguous.
    // For simplicity, we can return the point on the plane closest to the origin.
    DirectX::XMFLOAT3 supportPoint = normal;
    DirectX::XMVECTOR dirVector = DirectX::XMLoadFloat3(&direction);
    DirectX::XMVECTOR normalVector = DirectX::XMLoadFloat3(&normal);
    float dotProduct = DirectX::XMVectorGetX(DirectX::XMVector3Dot(dirVector, normalVector));

    if (dotProduct > 0)
    {
        supportPoint.x *= d;
        supportPoint.y *= d;
        supportPoint.z *= d;
    }
    else
    {
        supportPoint.x *= -d;
        supportPoint.y *= -d;
        supportPoint.z *= -d;
    }

    return supportPoint;
}

// Update the transform matrix
void PlaneCollider::updateTransform(const DirectX::XMFLOAT4X4& newTransform)
{
    transform = newTransform;

    // Recalculate the normal based on the transform
    DirectX::XMVECTOR newNormal = DirectX::XMVector3TransformNormal(DirectX::XMLoadFloat3(&normal), DirectX::XMLoadFloat4x4(&newTransform));
    DirectX::XMStoreFloat3(&normal, DirectX::XMVector3Normalize(newNormal));

    // Optionally, recalculate d based on the new transform, if needed
}

// Get the center of the plane in world space
DirectX::XMFLOAT3 PlaneCollider::getCenter() const
{
    // The center of a plane is not well-defined since it extends infinitely, 
    // but for practical purposes, we can return the point on the plane closest to the origin.
    return DirectX::XMFLOAT3(normal.x * d, normal.y * d, normal.z * d);
}

// Get the bounding radius of the plane
float PlaneCollider::getBoundingRadius() const
{
    // A plane doesn't have a bounding radius in the traditional sense since it's infinite,
    // but you might define a practical bounding radius based on its application.
    // For now, we'll return a large number to signify its "infinite" extent.
    return FLT_MAX; // Represents an infinitely large bounding radius
}
