/// @file     BehaviorSystem.h
/// @author   Name (first.last@digipen.edu)
///
/// @brief    Example System meant to be copy-pasted when creating new Systems
#pragma once

#define BEHAVIORSYSTEM_H

#include "System.h"
#include <vector>

class Behavior;
template < typename BehaviorType >
class BehaviorSystem : public System
{
private:

    /// @brief      Gets called once when this System is added to the Engine
    virtual void OnInit() override;

    /// @brief      Gets called once every simulation frame. Use this function for anything that affects the simulation.
    virtual void OnFixedUpdate() override;

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

    /// @brief      Gets called whenever a scene is exited
    virtual void OnSceneExit() override;

    /// @brief Loads the config data of this System
    /// @param configData the JSON object with all of the configData for this System
    virtual void Load(rapidjson::Value const& configData) override {}

    /// @brief      Constructs the BehaviorSystem 
    BehaviorSystem() = default;

    /// @brief      The singleton instance of BehaviorSystem  
    static BehaviorSystem< BehaviorType >* instance;


public:
    /// @brief      Adds a new Behavior to the system
    void AddBehavior(BehaviorType* behavior);
    ///@brief       Removes a Behavior from the system
    void RemoveBehavior(BehaviorType* behavior);

    /// @brief      Gets the instance of BehaviorSystem
    /// @return     BehaviorSystem pointer: new or existing instance of this system
    static BehaviorSystem< BehaviorType >* GetInstance();
    std::vector< BehaviorType* >& GetBehaviors() const;

private:
    std::vector< Behavior* > behaviorsList;

    // Prevent copying
    BehaviorSystem( BehaviorSystem& other ) = delete;
    void operator=( const BehaviorSystem& ) = delete;
};

#ifndef BEHAVIORSYSTEM_C
#include "BehaviorSystem.cpp"
#endif