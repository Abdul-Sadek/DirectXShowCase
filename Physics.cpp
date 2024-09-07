#include "Physics.h"
#include "imgui/imgui.h"

Physics* Physics::physics_engine = nullptr;

Physics::Physics()
{
}

Physics::~Physics()
{
    Physics::physics_engine = nullptr;
}

void Physics::initialize()
{
    if (Physics::physics_engine) throw std::exception("Physics Engine already created");
    Physics::physics_engine = new Physics();
}

void Physics::shutdown()
{
    if (!Physics::physics_engine) return;
    delete Physics::physics_engine;
}

Physics* Physics::get()
{
    return physics_engine;
}

void Physics::update(float deltaTime)
{
    this->deltaTime = deltaTime;

    applyGravity();

    for (auto rb : rigidbodies)
    {
        rb->update(deltaTime, airResistance);
    }

    if (enable_collision_detection)
    {
        detectCollisions();
    }
}

void Physics::applyGravity()
{
    for (auto rb : rigidbodies)
    {
        if (rb->isAffectedByGravity())
        {
            DirectX::XMFLOAT3 gravityForce;
            gravityForce.x = gravity.x * rb->mass;
            gravityForce.y = gravity.y * rb->mass;
            gravityForce.z = gravity.z * rb->mass;

            rb->applyForce(gravityForce);
        }
    }
}

void Physics::addRigidbody(RigidBody* rb)
{
    if (std::find(rigidbodies.begin(), rigidbodies.end(), rb) == rigidbodies.end())
    {
        IMGUI_DEBUG_LOG("Adding Rigidbody..."); // Debug logging
        rigidbodies.push_back(rb);
    }
    else
    {
        IMGUI_DEBUG_LOG("Rigidbody already in list!");
    }
}

void Physics::removeRigidbody(RigidBody* rb)
{
    auto it = std::remove(rigidbodies.begin(), rigidbodies.end(), rb);
    if (it != rigidbodies.end())
    {
        rigidbodies.erase(it, rigidbodies.end());
    }
}

void Physics::detectCollisions()
{
    collisions.clear();

    for (size_t i = 0; i < rigidbodies.size(); ++i)
    {
        for (size_t j = i + 1; j < rigidbodies.size(); ++j)
        {
            if (checkCollision(rigidbodies[i], rigidbodies[j]))
            {
                Collision* collision = new Collision(rigidbodies[i], rigidbodies[j], DirectX::XMFLOAT3(0.0f, 0.0f, 0.0f), DirectX::XMFLOAT3(0.0f, 0.0f, 0.0f), float(0.0f));
                collisions.push_back(collision);
                resolveCollision(collision);
            }
        }
    }
}

bool Physics::checkCollision(RigidBody* rb1, RigidBody* rb2)
{
    if (rb1 && rb2 && rb1->collisionShape && rb2->collisionShape)
    {
        //calculating contact points and normal
        DirectX::XMFLOAT3 p1 = rb1->collisionShape->getSupportPoint(DirectX::XMFLOAT3(0.0f, 0.0f, 0.0f));
        DirectX::XMFLOAT3 p2 = rb2->collisionShape->getSupportPoint(DirectX::XMFLOAT3(0.0f, 0.0f, 0.0f));
        DirectX::XMFLOAT3 contactNormal;
        contactNormal.x = p1.x - p2.x;
        contactNormal.y = p1.y - p2.y;
        contactNormal.z = p1.z - p2.z;
        // Normalize
        float length = sqrt(contactNormal.x * contactNormal.x + contactNormal.y * contactNormal.y + contactNormal.z * contactNormal.z);
        contactNormal.x /= length;
        contactNormal.y /= length;
        contactNormal.z /= length;
        //float penetrationDepth = length;  // The distance between p1 and p2
        DirectX::XMFLOAT3 contactPoint;
        contactPoint.x = (p1.x + p2.x) / 2.0f;
        contactPoint.y = (p1.y + p2.y) / 2.0f;
        contactPoint.z = (p1.z + p2.z) / 2.0f;
        // Create a temporary collision object to test for intersection
        Collision collision(rb1, rb2, contactPoint, contactNormal, 0.01f);
        return collision.isColliding();
    }
    // If either rigid body or their collision shapes are null, return false
    return false;
}

void Physics::resolveCollision(Collision* collision)
{
    // Implement collision resolution logic
    // This is a placeholder and should be replaced with your actual collision resolution code.
    collision->resolve();
}

void Physics::setGravity(const DirectX::XMFLOAT3& newGravity)
{
    gravity = newGravity;
}

void Physics::setAirResistance(float resistance)
{
    airResistance = resistance;
}

void Physics::enableCollisionDetection(bool enable)
{
    enable_collision_detection = enable;
}

void Physics::logPhysicsState()
{
    IMGUI_DEBUG_LOG("Physics State Log:");
    IMGUI_DEBUG_LOG("Gravity: (%f,%f)", gravity.x ,gravity.y);
    IMGUI_DEBUG_LOG("Air Resistance: %f", airResistance);
    IMGUI_DEBUG_LOG("Number of Rigidbodies: %s", rigidbodies.size());
    IMGUI_DEBUG_LOG("Number of Collisions: %s", collisions.size());

    for (const auto& rb : rigidbodies)
    {
        IMGUI_DEBUG_LOG("Rigidbody - Position: (%f,%f,%f)", rb->position.x, rb->position.y, rb->position.z);
    }
}