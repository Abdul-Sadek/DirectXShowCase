#pragma once

#include <directxmath.h>
class CollisionShape;

class BoxCollider : public CollisionShape {
public:
    DirectX::XMFLOAT3 minCorner;
    DirectX::XMFLOAT3 maxCorner;

    bool intersects(const CollisionShape& other) const override;
    DirectX::XMFLOAT3 getSupportPoint(const DirectX::XMFLOAT3& direction) const override;
    // Other methods specific to a bounding box...
};

