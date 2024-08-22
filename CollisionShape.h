#pragma once
#include <directxmath.h>

class CollisionShape
{
public:
    // Transform Data
    DirectX::XMFLOAT4X4 transform;

    // Methods
    virtual bool intersects(const CollisionShape& other) const = 0;
    virtual DirectX::XMFLOAT3 getSupportPoint(const DirectX::XMFLOAT3& direction) const = 0;
    virtual void updateTransform(const DirectX::XMFLOAT4X4& newTransform) = 0;
    virtual DirectX::XMFLOAT3 getCenter() const = 0;
    virtual float getBoundingRadius() const = 0;
};

