#pragma once

#include "Collision.h"
#include "CollisionShape.h"

class Collision;
class CollisionShape;

class RigidBody
{
public:
    // Physics Properties
    float mass;
    DirectX::XMFLOAT3 velocity;
    DirectX::XMFLOAT3 acceleration;
    DirectX::XMFLOAT3 force;

    // Transform
    DirectX::XMFLOAT3 position;
    DirectX::XMFLOAT3 rotation;
    DirectX::XMFLOAT3 scale;

    // Collision Properties
    CollisionShape* collisionShape;

    // Constructor
    RigidBody(float mass, const DirectX::XMFLOAT3& velocity, const DirectX::XMFLOAT3& acceleration,
        const DirectX::XMFLOAT3& force, const DirectX::XMFLOAT3& position,
        const DirectX::XMFLOAT3& rotation, const DirectX::XMFLOAT3& scale, CollisionShape* collisionShape);

    // Methods
    void applyForce(const DirectX::XMFLOAT3& force);
    void update(float deltaTime, float airResistance);
    void integrate(float deltaTime);
    void setMass(float newMass);
    void setPosition(const DirectX::XMFLOAT3& newPosition);
    DirectX::XMFLOAT3 getVelocity() const;

    // Gravity flag
    bool affectedByGravity;

    // Gravity-related methods
    void setAffectedByGravity(bool affected);
    bool isAffectedByGravity() const;
};

