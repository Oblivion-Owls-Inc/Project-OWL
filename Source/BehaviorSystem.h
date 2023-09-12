/// @file     BehaviorSystem.h
/// @author   Name (first.last@digipen.edu)
///
/// @brief    Example System meant to be copy-pasted when creating new Systems
#pragma once
#include "System.h"


template <typename BehaviorType>
class BehaviorSystem : System
{
private:

    /// @brief      Gets called once when this System is added to the Engine
    virtual void OnInit() override {}

    /// @brief      Gets called once every simulation frame. Use this function for anything that affects the simulation.
    virtual void OnFixedUpdate() override {}

    /// @brief      Gets called once every graphics frame. Do not use this function for anything 
    ///             that affects the simulation.
    /// @param dt   The elapsed time in seconds since the previous frame
    virtual void OnUpdate(float dt) override {}

    /// @brief      Gets called once before the Engine closes
    virtual void OnExit() override {}

    /// @brief      Gets called whenever a new Scene is loaded
    virtual void OnSceneLoad() override {}

    /// @brief      Gets called whenever a scene is initialized
    virtual void OnSceneInit() override {}

    /// @brief      Gets called whenever a scene is exited
    virtual void OnSceneExit() override {}

    /// @brief      Constructs the BehaviorSystem 
    BehaviorSystem();

    /// @brief      The singleton instance of BehaviorSystem  
    static BehaviorSystem* instance;

public:

    /// @brief      Gets the instance of BehaviorSystem
    /// @return     BehaviorSystem pointer: new or existing instance of this system
    static BehaviorSystem* getInstance();

private:
    std::vector<T> behaviors;


    // MAY INCLUDE A DUPLICATE OF THE ABOVE FUNCTION WITH CONSTRUCTOR ARGUMENTS HERE


    // Prevent copying
    BehaviorSystem(BehaviorSystem& other) = delete;
    void operator=(const BehaviorSystem&) = delete;
};

