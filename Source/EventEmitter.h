/////////////////////////////////////////////////////////////////////////////////
/// @file       Event Emitter.h
/// @author     Jax Clayton (jax.clayton@digipen.edu)
/// @brief      Component that emits events on Init and Exit
/// @version    0.1
/// @date       4/6/2024
/// 
/// @copyright  Copyright (c) 2024 Digipen Institute of Technology
/////////////////////////////////////////////////////////////////////////////////

#pragma once
#include "Component.h"

/// @brief  Component that emits events on Init and Exit
class EventEmitter : public Component
{
//-----------------------------------------------------------------------------
public: // constructor / Destructor
//-----------------------------------------------------------------------------


    /// @brief  default constructor
    EventEmitter();


//-----------------------------------------------------------------------------
public: // methods
//-----------------------------------------------------------------------------


//-----------------------------------------------------------------------------
public: // accessors
//-----------------------------------------------------------------------------


//-----------------------------------------------------------------------------
public: // virtual override methods
//-----------------------------------------------------------------------------


    /// @brief  called once when entering the scene
    virtual void OnInit() override;

    /// @brief  called once when exiting the scene
    virtual void OnExit() override;


//-----------------------------------------------------------------------------
private: // members
//-----------------------------------------------------------------------------

    /// @brief the name of the event to emit on init
    std::string m_InitEventName;

    /// @brief the name of the event to emit on exit
    std::string m_ExitEventName;

//-----------------------------------------------------------------------------
private: // methods
//-----------------------------------------------------------------------------

    void EmitEvent(std::string EventName) const;

//-----------------------------------------------------------------------------
public: // inspection
//-----------------------------------------------------------------------------


    /// @brief  shows the inspector for EventEmitter
    virtual void Inspector() override;


//-----------------------------------------------------------------------------
private: // reading
//-----------------------------------------------------------------------------

    /// @brief  reads the event name from JSON
    /// @param  data    the JSON data to read
    void readEventName( nlohmann::ordered_json const& data );

    /// @brief reads the init event name from JSON
    /// @param data - the JSON data to read
    void readInitEventName( nlohmann::ordered_json const& data );

    /// @brief reads the exit event name from JSON
    /// @param data - the JSON data to read
    void readExitEventName( nlohmann::ordered_json const& data );

//-----------------------------------------------------------------------------
public: // reading / writing
//-----------------------------------------------------------------------------


    /// @brief  gets the map of read methods for this EventEmitter
    /// @return the map of read methods for this EventEmitter
    virtual ReadMethodMap< ISerializable > const& GetReadMethods() const override;


    /// @brief  writes this EventEmitter to JSON
    /// @return the JSON data of this EventEmitter
    virtual nlohmann::ordered_json Write() const override;

    
//-----------------------------------------------------------------------------
public: // copying
//-----------------------------------------------------------------------------


    /// @brief  clones this EventEmitter
    /// @return the newly created clone of this EventEmitter
    virtual EventEmitter* Clone() const override;


//-----------------------------------------------------------------------------
private: // copying
//-----------------------------------------------------------------------------


    /// @brief  copy-constructor for the EventEmitter
    /// @param  other   the other EventEmitter to copy
    EventEmitter( EventEmitter const& other );


    // disable assignment operator
    void operator =( EventEmitter const& ) = delete;


//-----------------------------------------------------------------------------
};

