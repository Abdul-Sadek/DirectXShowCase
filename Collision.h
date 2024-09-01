#pragma once

#include "RigidBody.h"
#include <directxmath.h>

class RigidBody;

class Collision
{
public:
    // Collision Objects
    RigidBody* objectA;
    RigidBody* objectB;

    // Collision Information
    DirectX::XMFLOAT3 contactPoint;
    DirectX::XMFLOAT3 contactNormal;
    mutable float penetrationDepth;

    // Constructor
    Collision(RigidBody* objA, RigidBody* objB,
        const DirectX::XMFLOAT3& contactPoint,
        const DirectX::XMFLOAT3& contactNormal,
        float penetrationDepth);

    // Methods
    void resolve();
    bool isColliding() const;
    DirectX::XMFLOAT3 getContactPoint() const;
    DirectX::XMFLOAT3 getContactNormal() const;
};

