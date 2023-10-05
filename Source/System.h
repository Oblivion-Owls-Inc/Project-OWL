/// @file       System.h
/// @author     Steve Bukowinski (steve.bukowinski@digipen.edu)
/// @brief      Base class for all Systems
/// @version    0.1
/// @date       2023-09-05
/// 
/// @copyright  Copyright (c) 2023

#pragma once

#include "Stream.h"

/// @brief  Base class for all Systems
class System
{

//-----------------------------------------------------------------------------
public: // virtual methods
//-----------------------------------------------------------------------------

    /// @brief  Gets called once when this System is added to the Engine
    virtual void OnInit() {};

    /// @brief  Gets called once every simulation frame. Use this function for anything that affects the simulation.
    virtual void OnFixedUpdate() {};

    /// @brief  Gets called once every graphics frame. Do not use this function for anything that affects the simulation.
    /// @param  dt  the elapsed time in seconds since the previous frame
    virtual void OnUpdate(float dt) {};

    /// @brief  Gets called once before the Engine closes
    virtual void OnExit() {};

    /// @brief  Gets called whenever a new Scene is loaded
    virtual void OnSceneLoad() {};

    /// @brief  Gets called whenever a scene is initialized
    virtual void OnSceneInit() {};

    /// @brief  Gets called whenever a scene is exited
    virtual void OnSceneExit() {};

    /// @brief Gets Called by the Debug system to display debug information
    virtual void DebugWindow() {};

//-----------------------------------------------------------------------------
public: // methods
//-----------------------------------------------------------------------------

    /// @brief Gets the unique name of this System
    _inline std::string GetName() const { return m_Name; }

//-----------------------------------------------------------------------------
public: // reading
//-----------------------------------------------------------------------------

    /// @brief  gets this System's read methods
    /// @return this System's read methods
    virtual ReadMethodMap< System > const& GetReadMethods() const = 0;

//-----------------------------------------------------------------------------
protected: // constructor
//-----------------------------------------------------------------------------
    
    /// @brief  Constructs a System
    System( std::string const& name ) :
        m_Name( name )
    {}

//-----------------------------------------------------------------------------
public: // singleton stuff
//-----------------------------------------------------------------------------
    
    // Prevent Systems from being copied
    System( System& other ) = delete;
    void operator=( const System& ) = delete;

//-----------------------------------------------------------------------------
private: // System Name
//-----------------------------------------------------------------------------
    /// @brief The Unique Name of this System to be used for Debug Window
    std::string m_Name;
};
