/////////////////////////////////////////////////////////////////////////////////
/// @file       Event Emitter.cpp
/// @author     Jax Clayton (jax.clayton@digipen.edu)
/// @brief      Component that emits events on Init and Exit
/// @version    0.1
/// @date       4/6/2024
/// 
/// @copyright  Copyright (c) 2024 Digipen Institute of Technology
/////////////////////////////////////////////////////////////////////////////////

#include "pch.h"
#include "EventEmitter.h"
#include "EventSystem.h"


//-----------------------------------------------------------------------------
// public: constructor / Destructor
//-----------------------------------------------------------------------------


    /// @brief  default constructor
EventEmitter::EventEmitter() :
    Component(typeid(EventEmitter))
{}


//-----------------------------------------------------------------------------
// public: methods
//-----------------------------------------------------------------------------


//-----------------------------------------------------------------------------
// public: accessors
//-----------------------------------------------------------------------------


//-----------------------------------------------------------------------------
// public: virtual override methods
//-----------------------------------------------------------------------------


    /// @brief  called once when entering the scene
void EventEmitter::OnInit()
{
    EmitEvent();
}

/// @brief  called once when exiting the scene
void EventEmitter::OnExit()
{
    EmitEvent();
}


//-----------------------------------------------------------------------------
// private: methods
//-----------------------------------------------------------------------------

void EventEmitter::EmitEvent() const
{
    Events()->BroadcastEvent< std::string >(m_EventName);
    Debug() << "Event Emitted: " << m_EventName << std::endl;
}

//-----------------------------------------------------------------------------
// public: inspection
//-----------------------------------------------------------------------------



/// @brief  shows the inspector for EventEmitter
void EventEmitter::Inspector()
{
    ImGui::InputText("Event Name", &m_EventName);
}


//-----------------------------------------------------------------------------
// private: reading
//-----------------------------------------------------------------------------

void EventEmitter::readEventName(nlohmann::ordered_json const& data)
{
    Stream::Read(m_EventName, data);
}

//-----------------------------------------------------------------------------
// public: reading / writing
//-----------------------------------------------------------------------------


    /// @brief  gets the map of read methods for this EventEmitter
    /// @return the map of read methods for this EventEmitter
ReadMethodMap< ISerializable > const& EventEmitter::GetReadMethods() const
{
    static ReadMethodMap< EventEmitter > const readMethods = {
        { "EventName", &EventEmitter::readEventName }
    };

    return (ReadMethodMap< ISerializable > const&)readMethods;
}


/// @brief  writes this EventEmitter to JSON
/// @return the JSON data of this EventEmitter
nlohmann::ordered_json EventEmitter::Write() const
{
    nlohmann::ordered_json json = nlohmann::ordered_json::object();

    json["EventName"] = m_EventName;

    return json;
}


//-----------------------------------------------------------------------------
// public: copying
//-----------------------------------------------------------------------------


    /// @brief  clones this EventEmitter
    /// @return the newly created clone of this EventEmitter
EventEmitter* EventEmitter::Clone() const
{
    return new EventEmitter(*this);
}


//-----------------------------------------------------------------------------
// private: copying
//-----------------------------------------------------------------------------


    /// @brief  copy-constructor for the EventEmitter
    /// @param  other   the other EventEmitter to copy
EventEmitter::EventEmitter(EventEmitter const& other) :
    Component(other), m_EventName(other.m_EventName)
{}


//-----------------------------------------------------------------------------
