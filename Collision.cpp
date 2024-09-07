#include "Collision.h"
#include <utility>
#include "imgui/imgui.h"

Collision::Collision(RigidBody* objA, RigidBody* objB,
    const DirectX::XMFLOAT3& contactPoint,
    const DirectX::XMFLOAT3& contactNormal,
    float penetrationDepth)
    : objectA(objA), objectB(objB),
    contactPoint(contactPoint),
    contactNormal(contactNormal),
    penetrationDepth(penetrationDepth)
{
}

void Collision::resolve(DirectX::XMFLOAT3& contactPoint, DirectX::XMFLOAT3& contactNormal, float& penetrationDepth)
{
    // Calculate the relative velocity
    DirectX::XMFLOAT3 relativeVelocity;
    relativeVelocity.x = objectB->velocity.x - objectA->velocity.x;
    relativeVelocity.y = objectB->velocity.y - objectA->velocity.y;
    relativeVelocity.z = objectB->velocity.z - objectA->velocity.z;

    // Calculate the velocity along the normal
    float velocityAlongNormal = relativeVelocity.x * contactNormal.x +
        relativeVelocity.y * contactNormal.y +
        relativeVelocity.z * contactNormal.z;

    // Do not resolve if velocities are separating
    if (velocityAlongNormal > 0)
    {
        return;
    }

    // Calculate restitution (assuming a basic coefficient of restitution)
    float restitution = 0.5f; // Placeholder value, this could be a property of the material or object

    // Calculate impulse scalar
    float impulseScalar = -(1 + restitution) * velocityAlongNormal;
    impulseScalar /= (1 / objectA->mass) + (1 / objectB->mass);

    // Apply impulse
    DirectX::XMFLOAT3 impulse;
    impulse.x = impulseScalar * contactNormal.x;
    impulse.y = impulseScalar * contactNormal.y;
    impulse.z = impulseScalar * contactNormal.z;

    objectA->velocity.x -= (1 / objectA->mass) * impulse.x;
    objectA->velocity.y -= (1 / objectA->mass) * impulse.y;
    objectA->velocity.z -= (1 / objectA->mass) * impulse.z;

    objectB->velocity.x += (1 / objectB->mass) * impulse.x;
    objectB->velocity.y += (1 / objectB->mass) * impulse.y;
    objectB->velocity.z += (1 / objectB->mass) * impulse.z;

    // Positional correction (to avoid sinking objects)
    const float percent = 0.2f; // Usually 20% to 80%
    const float slop = 0.01f; // Usually a small value
    float correctionMagnitude = std::max(penetrationDepth - slop, 0.0f) / ((1 / objectA->mass) + (1 / objectB->mass));
    DirectX::XMFLOAT3 correction;
    correction.x = correctionMagnitude * contactNormal.x * percent;
    correction.y = correctionMagnitude * contactNormal.y * percent;
    correction.z = correctionMagnitude * contactNormal.z * percent;

    objectA->position.x -= (1 / objectA->mass) * correction.x;
    objectA->position.y -= (1 / objectA->mass) * correction.y;
    objectA->position.z -= (1 / objectA->mass) * correction.z;

    objectB->position.x += (1 / objectB->mass) * correction.x;
    objectB->position.y += (1 / objectB->mass) * correction.y;
    objectB->position.z += (1 / objectB->mass) * correction.z;
}

bool Collision::isColliding() const
{
    if (objectA && objectB && objectA->collisionShape && objectB->collisionShape)
    {
        // Use the intersects method from the CollisionShape to determine if the objects are colliding
        float penetrationDepth = objectA->collisionShape->intersects(*objectB->collisionShape);
        if (penetrationDepth > 0.0f)
        {
            this->penetrationDepth = penetrationDepth;
            return true;
        }
    }
    // If there's no collision or one of the objects/shapes is missing, return false.
    return false;
}

DirectX::XMFLOAT3 Collision::getContactPoint() const
{
    return contactPoint;
}

DirectX::XMFLOAT3 Collision::getContactNormal() const
{
    return contactNormal;
}