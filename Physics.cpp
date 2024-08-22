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

void Physics::update(float deltaTime)
{
}

void Physics::applyGravity()
{
}

void Physics::addRigidbody(RigidBody* rb)
{
}

void Physics::removeRigidbody(RigidBody* rb)
{
}

void Physics::detectCollisions()
{
}

bool Physics::checkCollision(RigidBody* rb1, RigidBody* rb2)
{
    return false;
}

void Physics::resolveCollision(Collision* collision)
{
}

void Physics::setGravity(const DirectX::XMFLOAT3& newGravity)
{
}

void Physics::setAirResistance(float resistance)
{
}

void Physics::enableCollisionDetection(bool enable)
{
}

void Physics::logPhysicsState()
{
    IMGUI_DEBUG_LOG("");
}

Physics* Physics::get()
{
    return physics_engine;
}