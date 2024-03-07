/////////////////////////////////////////////////////////////////////////////////
/// @file       pause.cpp
/// @author     Jax Clayton (jax.clayton@digipen.edu)
/// @brief      Component that pauses the game when the PauseComponent button is pressed
/// @version    0.1
/// @date       3/6/2024
/// 
/// @copyright  Copyright (c) 2024 Digipen Institute of Technology
/////////////////////////////////////////////////////////////////////////////////


#include "pause.h"
#include "PauseSystem.h"
#include "BehaviorSystem.h"

//-----------------------------------------------------------------------------
// public: constructor / Destructor
//-----------------------------------------------------------------------------


    /// @brief  default constructor
PauseComponent::PauseComponent() :
    Behavior(typeid(PauseComponent))
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
void PauseComponent::OnInit()
{
    m_PauseButton.SetOwnerName(GetName());
    

    m_PauseListener.SetFilterFunction([&](std::string const& EventName) -> bool
    {
        return EventName == m_EventName;
    });

    m_PauseListener.SetResponseFunction([&](std::string const& EventName)
    {
        Pause()->SetRunning(!Pause()->GetRunning());
    });


    m_PauseListener.Init();
    m_PauseButton.Init();

    BehaviorSystem< PauseComponent >::GetInstance()->AddComponent(this);
}

/// @brief Called at a fixed framestep
void PauseComponent::OnFixedUpdate()
{

    if (!m_PauseButton)
    {
        return;
    }


    if (m_PauseButton->GetReleased())
    {
        Pause()->SetRunning(!Pause()->GetRunning());
    }
}

/// @brief  called once when exiting the scene
void PauseComponent::OnExit()
{
    m_PauseListener.Exit();
    BehaviorSystem< PauseComponent >::GetInstance()->RemoveComponent(this);
}


//-----------------------------------------------------------------------------
// private: methods
//-----------------------------------------------------------------------------


//-----------------------------------------------------------------------------
// public: inspection
//-----------------------------------------------------------------------------


    /// @brief  shows the inspector for PauseComponent
void PauseComponent::Inspector()
{
    m_PauseButton.Inspect("Pause Button");
    ImGui::InputText("Event Name", &m_EventName, ImGuiInputTextFlags_EnterReturnsTrue);
}


//-----------------------------------------------------------------------------
// private: reading
//-----------------------------------------------------------------------------

void PauseComponent::readPauseButton(nlohmann::ordered_json const& data)
{
    Stream::Read(m_PauseButton, data);
}

void PauseComponent::readEventName(nlohmann::ordered_json const& data)
{
    Stream::Read(m_EventName, data);
}

//-----------------------------------------------------------------------------
// public: reading / writing
//-----------------------------------------------------------------------------


    /// @brief  gets the map of read methods for this PauseComponent
    /// @return the map of read methods for this PauseComponent
ReadMethodMap< ISerializable > const& PauseComponent::GetReadMethods() const
{
    static ReadMethodMap< PauseComponent > const readMethods = {
        { "PauseButton", &PauseComponent::readPauseButton   },
        { "EventName"  , &PauseComponent::readEventName     }
    };

    return (ReadMethodMap< ISerializable > const&)readMethods;
}


/// @brief  Writes all PauseComponent data to a JSON file.
/// @return The JSON file containing the data.
nlohmann::ordered_json PauseComponent::Write() const
{
    nlohmann::ordered_json json;

    json["PauseButton"] = m_PauseButton.Write();
    json["EventName"] = m_EventName;

    return json;
}


//-----------------------------------------------------------------------------
// public: copying
//-----------------------------------------------------------------------------


    /// @brief  clones this PauseComponent
    /// @return the newly created clone of this PauseComponent
PauseComponent* PauseComponent::Clone() const
{
    return new PauseComponent(*this);
}


//-----------------------------------------------------------------------------
// private: copying
//-----------------------------------------------------------------------------


    /// @brief  copy-constructor for the PauseComponent
    /// @param  other   the other PauseComponent to copy
PauseComponent::PauseComponent(PauseComponent const& other) :
    Behavior(other), m_PauseButton(other.m_PauseButton), m_EventName(other.m_EventName)
{}


//-----------------------------------------------------------------------------
