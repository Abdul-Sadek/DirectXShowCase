#pragma once

#include "CollisionShape.h"
#include <directxmath.h>
class CollisionShape;

class SphereCollider : public CollisionShape {
public:
    DirectX::XMFLOAT3 center;
    float radius;

    SphereCollider(float r) : radius(r) {}

    bool intersects(const CollisionShape& other) const override;
    DirectX::XMFLOAT3 getSupportPoint(const DirectX::XMFLOAT3& direction) const override;
    void updateTransform(const DirectX::XMFLOAT4X4& newTransform) override;
    DirectX::XMFLOAT3 getCenter() const override;
    float getBoundingRadius() const override;
};


