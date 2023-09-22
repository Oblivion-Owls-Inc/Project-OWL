#pragma once

#include "System.h"
#include "RigidBody.h"
#include "Transform.h"
#include <vector>

class Collider;
class Behavior;

class PhysicsSystem : public System
{
    private:

        PhysicsSystem();

    public:
        /// @brief      The singleton instance of BehaviorSystem  
        static PhysicsSystem* instance;

        /// @brief      Gets called once when this System is added to the Engine
        virtual void OnInit() override;

        /// @brief      Gets called once every simulation frame. Use this function for anything that affects the simulation.
        virtual void OnFixedUpdate() override;

        /// @brief      Gets called whenever a scene is exited
        virtual void OnSceneExit() override;

        /// @brief      Gets called once every graphics frame. Do not use this function for anything 
        ///             that affects the simulation.
        /// @param dt   The elapsed time in seconds since the previous frame
        virtual void OnUpdate(float dt) override;

        /// @brief      Gets called once before the Engine closes
        virtual void OnExit() override;


        /// @brief Loads the config data of this System
        /// @param configData the JSON object with all of the configData for this System
        virtual void Load(rapidjson::Value const& configData) override;

        /// @brief      Gets the instance of PhysicsSystem
        /// @return     PhysicsSystem pointer: new or existing instance of this system
        static PhysicsSystem* getInstance();

        /// @brief      Adds a new Behavior to the system
        void AddBehavior(Behavior *behavior);
        ///@brief       Removes a Behavior from the system
        void RemoveBehavior(Behavior *behavior);
        ///@brief       Adds a new Collider to the system
        void AddCollider(Collider *collider);
        ///@brief       Removes a Collider from the system
        void RemoveCollider(Collider *collider);

    private:
    std::vector<Behavior *> m_behaviors;
    std::vector<Collider *> m_colliders;

    // Prevent copying
    PhysicsSystem(PhysicsSystem& other) = delete;
    void operator=(const PhysicsSystem&) = delete;

    /// @brief      Gets called whenever a new Scene is loaded
    virtual void OnSceneLoad() override {}

    /// @brief      Gets called whenever a scene is initialized
    virtual void OnSceneInit() override {}
};
