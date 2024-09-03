#pragma once

#include "CollisionShape.h"
#include "BoxCollider.h"
#include "PlaneCollider.h"

class BoxCollider;
class PlaneCollider;

class SphereCollider : public CollisionShape {
public:
    DirectX::XMFLOAT3 center;
    float radius;

    SphereCollider(float r) : radius(r) {}

    bool intersects(const CollisionShape& other) const;
    bool intersectsBox(const BoxCollider& box) const;
    bool intersectsPlane(const PlaneCollider& plane) const;
    bool intersectsSphere(const SphereCollider& sphere) const;

    DirectX::XMFLOAT3 getSupportPoint(const DirectX::XMFLOAT3& direction) const;
    void updateTransform(const DirectX::XMFLOAT4X4& newTransform);
    DirectX::XMFLOAT3 getCenter() const;
    float getBoundingRadius() const;
};


