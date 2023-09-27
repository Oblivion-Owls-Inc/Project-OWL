/// @file CollisionSystem.h
/// @author Steve Bukowinski (steve.bukowinski@digipen.edu)
/// @brief Example System meant to be copy-pasted when creating new Systems
/// @version 0.1
/// @date 2023-09-05
/// 
/// @copyright Copyright (c) 2023

#pragma once

#include "System.h"
#include <vector>

class Collider;
/// @brief Example System meant to be copy-pasted when creating new Systems
class CollisionSystem : public System
{
    public: // singleton stuff

        /// @brief gets the instance of CollisionSystem
        /// @return the instance of the CollisionSystem
        static CollisionSystem* GetInstance();

        // Prevent copying
        CollisionSystem(CollisionSystem& other) = delete;
        void operator=(const CollisionSystem&) = delete;

        void addCollider(Collider* collider);

		void removeCollider(Collider* collider);

        /// @brief Gets called once every simulation frame. Use this function for anything that affects the simulation.
        virtual void OnFixedUpdate() override;

        void checkCollisions();



    private: // unused virtual overrides

        /// @brief Gets called once when this System is added to the Engine
        virtual void OnInit() override {}



        /// @brief Gets called once every graphics frame. Do not use this function for anything that affects the simulation.
        /// @param dt the elapsed time in seconds since the previous frame
        virtual void OnUpdate(float dt) override {}


        /// @brief Gets called once before the Engine closes
        virtual void OnExit() override {}


        /// @brief Gets called whenever a new Scene is loaded
        virtual void OnSceneLoad() override {}


        /// @brief Gets called whenever a scene is initialized
        virtual void OnSceneInit() override {}


        /// @brief Gets called whenever a scene is exited
        virtual void OnSceneExit() override {}

//-----------------------------------------------------------------------------
private: // reading
//-----------------------------------------------------------------------------

    /// @brief map of the CollisionSystem read methods
    static ReadMethodMap< CollisionSystem > const s_ReadMethods;

    /// @brief  gets this System's read methods
    /// @return this System's read methods
    virtual ReadMethodMap< System > const& GetReadMethods() const override;

    private: // singleton stuff

        /// @brief Constructs the CollisionSystem
        CollisionSystem();

        /// @brief The singleton instance of CollisionSystem
        static CollisionSystem* instance;

        std::vector<Collider*> colliderList;


};

