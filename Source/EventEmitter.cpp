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
    if (!m_InitEventName.empty())
    {
        EmitEvent(m_InitEventName);
    }
}

/// @brief  called once when exiting the scene
void EventEmitter::OnExit()
{
    if (!m_ExitEventName.empty())
    {
        EmitEvent(m_ExitEventName);
    }
}


//-----------------------------------------------------------------------------
// private: methods
//-----------------------------------------------------------------------------

void EventEmitter::EmitEvent(std::string EventName) const
{
    Events()->BroadcastEvent< std::string >(EventName);
    Debug() << "Event Emitted: " << EventName << std::endl;
}

//-----------------------------------------------------------------------------
// public: inspection
//-----------------------------------------------------------------------------



/// @brief  shows the inspector for EventEmitter
void EventEmitter::Inspector()
{
    ImGui::InputText("Init Event Name", &m_InitEventName);
    ImGui::Separator();
    ImGui::InputText("Exit Event Name", &m_ExitEventName);
}


//-----------------------------------------------------------------------------
// private: reading
//-----------------------------------------------------------------------------

void EventEmitter::readInitEventName(nlohmann::ordered_json const& data)
{
    Stream::Read(m_InitEventName, data);
}

void EventEmitter::readExitEventName(nlohmann::ordered_json const& data)
{
    Stream::Read(m_ExitEventName, data);
}

//-----------------------------------------------------------------------------
// public: reading / writing
//-----------------------------------------------------------------------------


    /// @brief  gets the map of read methods for this EventEmitter
    /// @return the map of read methods for this EventEmitter
ReadMethodMap< ISerializable > const& EventEmitter::GetReadMethods() const
{
    static ReadMethodMap< EventEmitter > const readMethods = {
        { "InitEventName", &EventEmitter::readInitEventName },
        { "ExitEventName", &EventEmitter::readExitEventName }
    };

    return (ReadMethodMap< ISerializable > const&)readMethods;
}


/// @brief  writes this EventEmitter to JSON
/// @return the JSON data of this EventEmitter
nlohmann::ordered_json EventEmitter::Write() const
{
    nlohmann::ordered_json json = nlohmann::ordered_json::object();

    json["InitEventName"] = m_InitEventName;
    json["ExitEventName"] = m_ExitEventName;

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
    Component(other), 
    m_InitEventName(other.m_InitEventName), 
    m_ExitEventName(other.m_ExitEventName)
{}


//-----------------------------------------------------------------------------
