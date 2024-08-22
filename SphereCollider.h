#pragma once

#include <directxmath.h>
class CollisionShape;

class SphereCollider : public CollisionShape {
public:
    DirectX::XMFLOAT3 center;
    float radius;

    bool intersects(const CollisionShape& other) const override;
    DirectX::XMFLOAT3 getSupportPoint(const DirectX::XMFLOAT3& direction) const override;
    // Other methods specific to a bounding sphere...
};

