#pragma once

#include "RigidBody.h"
#include "Collision.h"
#include <vector>

class Physics
{
public:
    // Initialization and Cleanup
    Physics();
    ~Physics();

    static void initialize();
    static void shutdown();
    static Physics* get();

    // Update Loop
    void update(float deltaTime);
    void applyGravity();

    // Rigidbody Management
    void addRigidbody(RigidBody* rb);
    void removeRigidbody(RigidBody* rb);

    // Collision Detection
    void detectCollisions();
    bool checkCollision(RigidBody* rb1, RigidBody* rb2);
    void resolveCollision(Collision* collision);

    // Physics Settings
    void setGravity(const DirectX::XMFLOAT3& newGravity);
    void setAirResistance(float resistance);
    void enableCollisionDetection(bool enable);

    //for debugging
    void logPhysicsState();

private:
    static Physics* physics_engine;
    DirectX::XMFLOAT3 gravity;
    float deltaTime;
    std::vector<RigidBody*> rigidbodies;
    std::vector<Collision*> collisions;
    float airResistance;
    bool enable_collision_detection;
};

