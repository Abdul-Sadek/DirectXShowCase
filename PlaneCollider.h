#pragma once

#include "CollisionShape.h"
#include <directxmath.h>
class CollisionShape;

class PlaneCollider : public CollisionShape {
public:
    // Plane Equation Components
    DirectX::XMFLOAT3 normal;
    float distance;

    // Constructor
    PlaneCollider(const DirectX::XMFLOAT3& normal, float distance)
        : normal(normal), distance(distance) {}

    // CollisionShape Interface Methods
    bool intersects(const CollisionShape& other) const override;
        // Implement collision logic with other shapes like Sphere, AABB, etc.
        // For example, collision detection with a sphere:
        // if (SphereCollider* sphere = dynamic_cast<SphereCollider*>(&other)) {
        //     return this->intersectsSphere(*sphere);
        // }
        //return false;

    DirectX::XMFLOAT3 getSupportPoint(const DirectX::XMFLOAT3& direction) const override;
        // For a plane, return a point on the plane. This is somewhat arbitrary for a plane,
        // but you could return the point at 'distance' along the plane normal.
        //return { normal.x * distance, normal.y * distance, normal.z * distance };

    void updateTransform(const DirectX::XMFLOAT4X4& newTransform) override;
        // Update the plane's normal based on the transformation matrix.
        // This might involve transforming the normal vector by the rotation part of the matrix.
        //DirectX::XMVECTOR normalVec = DirectX::XMLoadFloat3(&normal);
        //normalVec = DirectX::XMVector3TransformNormal(normalVec, DirectX::XMLoadFloat4x4(&newTransform));
        //DirectX::XMStoreFloat3(&normal, normalVec);

        // Update distance if the plane's position changes, though this is rare.

    DirectX::XMFLOAT3 getCenter() const override;
        // For a plane, the concept of a "center" is not applicable. You might return a point
        // on the plane or just the origin.
        //return { 0.0f, 0.0f, 0.0f };

    float getBoundingRadius() const override;
        // A plane is infinite, so it doesn't have a bounding radius. You could return infinity or a large value.
        //return FLT_MAX;

    // Plane-Specific Methods
    float distanceToPoint(const DirectX::XMFLOAT3& point) const {
        // Calculate the distance from a point to the plane using the plane equation.
        return DirectX::XMVector3Dot(DirectX::XMLoadFloat3(&normal), DirectX::XMLoadFloat3(&point)).m128_f32[0] - distance;
    }

    bool isPointAbove(const DirectX::XMFLOAT3& point) const {
        // Determine if a point is above (in the direction of the normal) the plane.
        return distanceToPoint(point) > 0;
    }
};