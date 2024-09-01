#pragma once

#include "CollisionShape.h"
#include <directxmath.h>
class CollisionShape;

class PlaneCollider : public CollisionShape {
public:
    // Plane properties
    DirectX::XMFLOAT3 normal;  // Normal of the plane
    float d;                  // Distance from the origin along the normal

    // Constructor
    PlaneCollider(const DirectX::XMFLOAT3& normal, float d);

    // Override methods from CollisionShape
    bool intersects(const CollisionShape& other) const override;
    DirectX::XMFLOAT3 getSupportPoint(const DirectX::XMFLOAT3& direction) const override;
    void updateTransform(const DirectX::XMFLOAT4X4& newTransform) override;
    DirectX::XMFLOAT3 getCenter() const override;
    float getBoundingRadius() const override;
};