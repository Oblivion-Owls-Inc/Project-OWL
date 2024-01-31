/// @file       EventSystem.h
/// @author     Steve Bukowinski (steve.bukowinski@digipen.edu)
/// @brief      Example System meant to be copy-pasted when creating new Systems
/// @version    0.1
/// @date       2023-09-05
/// 
/// @copyright  Copyright (c) 2023

#pragma once

#include "System.h"

template < typename EventType >
class EventListener;

/// @brief  Example System meant to be copy-pasted when creating new Systems
class EventSystem : public System
{
//-----------------------------------------------------------------------------
public: // methods
//-----------------------------------------------------------------------------

    
    /// @brief  adds an event listener to the event system
    /// @tparam EventType       the type of event the listener listens for
    /// @param  eventListener   the event listener to add to the event system
    template < typename EventType >
    void AddEventListener( EventListener< EventType >* eventListener );

    
//-----------------------------------------------------------------------------
public: // virtual override methods
//-----------------------------------------------------------------------------


//-----------------------------------------------------------------------------
private: // members
//-----------------------------------------------------------------------------


//-----------------------------------------------------------------------------
private: // methods
//-----------------------------------------------------------------------------


//-----------------------------------------------------------------------------
public: // inspection
//-----------------------------------------------------------------------------


    /// @brief Gets Called by the Debug system to display debug information
    virtual void DebugWindow() override;


//-----------------------------------------------------------------------------
public: // reading / writing
//-----------------------------------------------------------------------------


    /// @brief  gets this System's read methods
    /// @return this System's read methods
    virtual ReadMethodMap< ISerializable > const& GetReadMethods() const override;


    /// @brief  writes this EventSystem to JSON
    /// @return the JSON data of this EventSystem
    virtual nlohmann::ordered_json Write() const override;


//-----------------------------------------------------------------------------
public: // singleton stuff
//-----------------------------------------------------------------------------


    /// @brief  gets the instance of EventSystem
    /// @return the instance of the EventSystem
    static EventSystem* GetInstance();


//-----------------------------------------------------------------------------
private: // singleton stuff
//-----------------------------------------------------------------------------


    /// @brief  Constructs the EventSystem
    EventSystem();

    // Prevent copying
    EventSystem( EventSystem const& ) = delete;
    void operator =( EventSystem const& ) = delete;


//-----------------------------------------------------------------------------
};

