/// @file       System.h
/// @author     Steve Bukowinski (steve.bukowinski@digipen.edu)
/// @brief      Base class for all Systems
/// @version    0.1
/// @date       2023-09-05
/// 
/// @copyright  Copyright (c) 2023 Digipen Institute of Technology

#pragma once

#include "ISerializable.h"

#include "basics.h"
#include "Stream.h"

/// @brief  Base class for all Systems
class System : public ISerializable
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

    /// @brief  Gets called whenever a scene is initialized
    virtual void OnSceneInit() {};

    /// @brief  Gets called whenever a scene is exited
    virtual void OnSceneExit() {};

    /// @brief Gets Called by the Debug system to display debug information
    virtual void DebugWindow() {};

//-----------------------------------------------------------------------------
public: // accessors
//-----------------------------------------------------------------------------


    /// @brief Gets the unique name of this System
    std::string const& GetName() const { return m_Name; }

    /// @brief  gets whether this System is enabled
    /// @return whether this System is enabled
    bool GetEnabled() const { return m_Enabled; }

    bool GetDebugEnabled() const { return m_ShowDebugWindow; }

    /// @brief  sets whether this System is enabled
    /// @param  enabled whether this System is enabled
    void SetEnabled( bool enabled ) { m_Enabled = enabled; }

    /// @brief Sets if the Debug Window should be shown
    /// @param enable - whether the debug window should be shown
    virtual void SetDebugEnable(bool enable) { m_ShowDebugWindow = enable; }

    /// @brief  gets this System's ID
    /// @return this System's ID
    unsigned GetId() const { return m_Id; }


//-----------------------------------------------------------------------------
protected: // constructor
//-----------------------------------------------------------------------------
    
    /// @brief  Constructs a System
    System( std::string const& name ) :
        m_Name( name )
    {
        m_Id = GetUniqueId();
    }

//-----------------------------------------------------------------------------
public: // singleton stuff
//-----------------------------------------------------------------------------
    
    // Prevent Systems from being copied
    System( System& other ) = delete;
    void operator=( const System& ) = delete;

//-----------------------------------------------------------------------------
private: // member variables
//-----------------------------------------------------------------------------

    /// @brief The Unique Name of this System to be used for Debug Window
    std::string m_Name = "";

    /// @brief  the Id of this System
    unsigned m_Id = 0;

    /// @brief  whether this System will update
    bool m_Enabled = true;

    bool m_ShowDebugWindow = false;

//-----------------------------------------------------------------------------
};
