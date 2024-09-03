#pragma once

#include "CollisionShape.h"
#include "PlaneCollider.h"
#include "SphereCollider.h"

class PlaneCollider;
class SphereCollider;

class BoxCollider : public CollisionShape {
public:
    DirectX::XMFLOAT3 extents;

    BoxCollider(const DirectX::XMFLOAT3& boxExtents);

    bool intersects(const CollisionShape& other) const;
    bool intersectsBox(const CollisionShape& box) const;
    bool intersectsPlane(const CollisionShape& plane) const;
    bool intersectsSphere(const CollisionShape& sphere) const;

    
    // Get the farthest point on the box in the given direction
    DirectX::XMFLOAT3 getSupportPoint(const DirectX::XMFLOAT3& direction) const;

    // Update the transform matrix
    void updateTransform(const DirectX::XMFLOAT4X4& newTransform);

    // Get the center of the box in world space
    DirectX::XMFLOAT3 getCenter() const;

    // Get the bounding radius of the box
    float getBoundingRadius() const;
};

