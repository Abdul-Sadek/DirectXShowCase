#pragma once

#include "CollisionShape.h"
#include <directxmath.h>
class CollisionShape;

class BoxCollider : public CollisionShape {
public:
    DirectX::XMFLOAT3 extents;

    BoxCollider(const DirectX::XMFLOAT3& boxExtents);

    // Check if this box intersects with another shape
    bool intersects(const CollisionShape& other) const override;

    // Get the farthest point on the box in the given direction
    DirectX::XMFLOAT3 getSupportPoint(const DirectX::XMFLOAT3& direction) const override;

    // Update the transform matrix
    void updateTransform(const DirectX::XMFLOAT4X4& newTransform) override;

    // Get the center of the box in world space
    DirectX::XMFLOAT3 getCenter() const override;

    // Get the bounding radius of the box
    float getBoundingRadius() const override;
};

