#pragma once
#include "BehaviorSystem.h"
#include "RidgedBody.h"
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
    std::vector<RidgedBody *> m_ridgedBodies;
    std::vector<Transform *> m_transforms;
    std::vector<Collider *> m_colliders;
};
