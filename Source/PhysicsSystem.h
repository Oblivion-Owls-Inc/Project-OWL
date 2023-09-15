#pragma once
#include "BehaviorSystem.h"
#include "RigidBody.h"
#include "Transform.h"
#include "Collider.h"
#include <vector>


class PhysicsSystem : public BehaviorSystem<PhysicsSystem>
{
    public:
        PhysicsSystem();
        void OnFixedUpdate() override;
        void OnSceneExit() override;

    private:
    std::vector<RigidBody *> m_ridgedBodies;
    std::vector<Transform *> m_transforms;
    //std::vector m_colliders;
};
