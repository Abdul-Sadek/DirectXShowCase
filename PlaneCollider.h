#pragma once

#include "CollisionShape.h"
#include "BoxCollider.h"
#include "SphereCollider.h"

class BoxCollider;
class SphereCollider;

class PlaneCollider : public CollisionShape {
public:
    // Plane properties
    DirectX::XMFLOAT3 normal;  // Normal of the plane
    float d;                  // Distance from the origin along the normal

    // Constructor
    PlaneCollider(const DirectX::XMFLOAT3& normal, float d);

    bool intersects(const CollisionShape& other) const;
    bool intersectsBox(const BoxCollider& box) const;
    bool intersectsPlane(const PlaneCollider& plane) const;
    bool intersectsSphere(const SphereCollider& sphere) const;

    DirectX::XMFLOAT3 getSupportPoint(const DirectX::XMFLOAT3& direction) const;
    void updateTransform(const DirectX::XMFLOAT4X4& newTransform);
    DirectX::XMFLOAT3 getCenter() const;
    float getBoundingRadius() const;
};