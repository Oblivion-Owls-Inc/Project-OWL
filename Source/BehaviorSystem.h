///*****************************************************************/
/// @file	 BehaviorSystem.h
/// @Author  Jax Clayton (jax.clayton@digipen.edu)
/// @date	 9/15/2021
/// @brief   RigidBody class header
/// @details This class contains the RigidBody component
///*****************************************************************/
#pragma once
#define BEHAVIORSYSTEM_H


///*****************************************************************/
/// Includes:
///*****************************************************************/
#include "System.h"
#include <vector>
#include "Behavior.h"

///*****************************************************************/
/// BehaviorSystem class
/// @brief This is a Templated BehaviorSystem Class that inherits 
///        from System
///*****************************************************************/
template < typename BehaviorType >
class BehaviorSystem : public System
{
private:

    /// @brief      Gets called once every simulation frame. Use this function for anything that affects the simulation.
    virtual void OnFixedUpdate() override;

    /// @brief      Gets called once every graphics frame. Do not use this function for anything 
    ///             that affects the simulation.
    /// @param dt   The elapsed time in seconds since the previous frame
    virtual void OnUpdate(float dt) override;

    virtual void DebugWindow() override;


public:

    /// @brief      Adds a new Behavior to the system
    void AddBehavior(BehaviorType* behavior);
    ///@brief       Removes a Behavior from the system
    void RemoveBehavior(BehaviorType* behavior);

    std::vector< BehaviorType* >& GetBehaviors() const;

    ///

    /// @brief      Gets the instance of BehaviorSystem
    /// @return     BehaviorSystem pointer: new or existing instance of this system
    static BehaviorSystem< BehaviorType >* GetInstance();

private:
    std::vector< Behavior* > m_BehaviorList;

    /// @brief      Constructs the BehaviorSystem 
    BehaviorSystem();

    /// @brief      The singleton instance of BehaviorSystem  
    static BehaviorSystem< BehaviorType >* s_Instance;

    // Prevent copying
    BehaviorSystem(BehaviorSystem& other) = delete;
    void operator=(const BehaviorSystem&) = delete;

    static bool s_ShowBehaviorSystemList;
};

template < typename BehaviorType >
__inline BehaviorSystem< BehaviorType >* Behaviors()
{
    return BehaviorSystem<BehaviorType>::GetInstance();
}

#ifndef BEHAVIORSYSTEM_C
#include "BehaviorSystem.cpp"
#endif