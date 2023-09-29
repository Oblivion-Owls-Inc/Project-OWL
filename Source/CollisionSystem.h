/// @file CollisionSystem.h
/// @author Jax Clayton  (jax.clayton@digipen.edu)
/// @brief The CollisionSystem is responsible for checking for collisions between Different Colliders
/// @version 0.1
/// @date 2023-09-15
/// 
/// @copyright  Digipen LLC (c) 2023

///*****************************************************************
/// Includes:
///*****************************************************************
#pragma once
#include "System.h"
#include <vector>

///*****************************************************************
/// @brief forward declaration of Collider
///*****************************************************************
class Collider;

///*****************************************************************
/// @brief CollisionSystem is responsible for checking for collisions
///        between Different Colliders
///*****************************************************************

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

        /// @brief The singleton s_Instance of CollisionSystem
        static CollisionSystem* s_Instance;

        std::vector<Collider*> m_ColliderList;


};

