#include "RigidBody.h"

RigidBody::RigidBody(float mass, const DirectX::XMFLOAT3& velocity, const DirectX::XMFLOAT3& acceleration,
    const DirectX::XMFLOAT3& force, const DirectX::XMFLOAT3& position,
    const DirectX::XMFLOAT3& rotation, const DirectX::XMFLOAT3& scale, CollisionShape* collisionShape)
    : mass(mass), velocity(velocity), acceleration(acceleration), force(force),
    position(position), rotation(rotation), scale(scale), collisionShape(collisionShape)
{
}

void RigidBody::applyForce(const DirectX::XMFLOAT3& force)
{
    // Add the applied force to the total force
    this->force.x += force.x;
    this->force.y += force.y;
    this->force.z += force.z;
}

void RigidBody::update(float deltaTime, float airResistance)
{
    // Apply air resistance
    velocity.x *= (1.0f - airResistance * deltaTime);
    velocity.y *= (1.0f - airResistance * deltaTime);
    velocity.z *= (1.0f - airResistance * deltaTime);

    // Integrate to update position
    integrate(deltaTime);
}

void RigidBody::integrate(float deltaTime)
{
    // Calculate acceleration using Newton's second law (F = ma, so a = F/m)
    acceleration.x = force.x / mass;
    acceleration.y = force.y / mass;
    acceleration.z = force.z / mass;

    // Update velocity using v = u + at (where u is the initial velocity)
    velocity.x += acceleration.x * deltaTime;
    velocity.y += acceleration.y * deltaTime;
    velocity.z += acceleration.z * deltaTime;

    // Update position using s = ut + 0.5at^2 (where u is the initial velocity)
    position.x += velocity.x * deltaTime + 0.5f * acceleration.x * deltaTime * deltaTime;
    position.y += velocity.y * deltaTime + 0.5f * acceleration.y * deltaTime * deltaTime;
    position.z += velocity.z * deltaTime + 0.5f * acceleration.z * deltaTime * deltaTime;
}

void RigidBody::setMass(float newMass)
{
    mass = newMass;
}

void RigidBody::setPosition(const DirectX::XMFLOAT3& newPosition)
{
    position = newPosition;
}

DirectX::XMFLOAT3 RigidBody::getVelocity() const
{
    return velocity;
}

void RigidBody::setAffectedByGravity(bool affected)
{
    affectedByGravity = affected;
}

bool RigidBody::isAffectedByGravity() const
{
    return affectedByGravity;
}