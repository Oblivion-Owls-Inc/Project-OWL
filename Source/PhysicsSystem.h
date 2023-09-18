#pragma once

#include "System.h"
#include "RigidBody.h"
#include "Transform.h"
#include "Collider.h"
#include <vector>


class PhysicsSystem : public System
{
    public:
        PhysicsSystem();

        /// @brief      Gets called once when this System is added to the Engine
        virtual void OnInit() override;

        /// @brief      Gets called once every simulation frame. Use this function for anything that affects the simulation.
        virtual void OnFixedUpdate() override;

        /// @brief      Gets called whenever a scene is exited
        virtual void OnSceneExit() override;

        /// @brief      The singleton instance of BehaviorSystem  
        static PhysicsSystem* instance;

        /// @brief      Gets called once every graphics frame. Do not use this function for anything 
        ///             that affects the simulation.
        /// @param dt   The elapsed time in seconds since the previous frame
        virtual void OnUpdate(float dt) override;

        /// @brief      Gets called once before the Engine closes
        virtual void OnExit() override;

        /// @brief      Gets called whenever a new Scene is loaded
        virtual void OnSceneLoad() override {}

        /// @brief      Gets called whenever a scene is initialized
        virtual void OnSceneInit() override {}


        /// @brief the Read Methods used in this System
        static std::map< std::string, ReadMethod< PhysicsSystem > > const ReadMethods;

        /// @brief Gets the read methods of this System
        /// @return the map of read methods of this System
        virtual std::map< std::string, ReadMethod< System > > const& GetReadMethods() override;


        /// @brief      Gets the instance of PhysicsSystem
        /// @return     PhysicsSystem pointer: new or existing instance of this system
        static PhysicsSystem* getInstance();

    private:
        std::vector<RigidBody *> m_rigidBodies;
        std::vector<Transform *> m_transforms;
        std::vector<Collider *> m_colliders;

        // Prevent copying
        PhysicsSystem(PhysicsSystem& other) = delete;
        void operator=(const PhysicsSystem&) = delete;
};
