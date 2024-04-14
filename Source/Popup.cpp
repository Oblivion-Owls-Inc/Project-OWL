/// @file       Popup.cpp
/// @author     Steve Bukowinski (steve.bukowinski@digipen.edu)
/// @brief      DESCRIPTION HERE
/// @version    0.1
/// @date       DATE HERE
/// 
/// @copyright  Copyright (c) 2024 Digipen Institute of Technology

#include "pch.h" // precompiled header has to be included first
#include "Popup.h"
#include "BehaviorSystem.h"
#include "EntitySystem.h"

//-----------------------------------------------------------------------------
// public: constructor / Destructor
//-----------------------------------------------------------------------------


    /// @brief  default constructor
    Popup::Popup() :
        Behavior( typeid(Popup) )
    {}


//-----------------------------------------------------------------------------
// public: methods
//-----------------------------------------------------------------------------


    /// @brief  gets whether the popup is open
    /// @return whether the popup is open
    bool Popup::GetOpen() const
    {
        return m_Popup != nullptr;
    }


    /// @brief  sets whether the popup is open
    /// @param  open    whether the popup is open
    void Popup::SetOpen( bool open )
    {    
        if ( open != GetOpen() )
        {
            togglePopup();
        }
    }


//-----------------------------------------------------------------------------
// public: accessors
//-----------------------------------------------------------------------------


//-----------------------------------------------------------------------------
// public: virtual override methods
//-----------------------------------------------------------------------------


/// @brief  called once when entering the scene
void Popup::OnInit()
{
    Entity* pop = Entities()->GetEntity(m_PopupEntity.GetName());
    
    if (pop)
    {
        pop->Destroy();
    }



    m_PopupEntity.SetOwnerName(GetName());
    m_PopupButton.SetOwnerName(GetName());

    /// Set the filter function for the listener
    m_Listener.SetFilterFunction([&](std::string const& EventName) -> bool
    {
        return EventName == m_EventName;
    });

    /// Set the Callback function for the listener
    m_Listener.SetResponseFunction([&](std::string const& EventName)
    {
        togglePopup();
    });

    m_Listener.Init();
    m_PopupEntity.Init();
    m_PopupButton.Init();

    BehaviorSystem< Popup >::GetInstance()->AddComponent(this);
    
}

/// @brief  called every frame
void Popup::OnFixedUpdate()
{
    if (!m_PopupButton)
        return;


    if (m_PopupButton->GetReleased())
    {
        togglePopup();
    }
}

/// @brief  called once when exiting the scene
void Popup::OnExit()
{

    m_Listener.Exit();
    BehaviorSystem< Popup >::GetInstance()->RemoveComponent(this);
}

//-----------------------------------------------------------------------------
// private: methods
//-----------------------------------------------------------------------------

/// @brief  Creates/Destroys the popup
void Popup::togglePopup()
{
    if (m_Popup != nullptr)
    {
        m_Popup->Destroy();
        m_Popup = nullptr;
    }
    else
    {
        m_Popup = m_PopupEntity->Clone();
        m_Popup->AddToScene();
    }
}

//-----------------------------------------------------------------------------
// public: inspection
//-----------------------------------------------------------------------------



/// @brief  shows the inspector for Popup
void Popup::Inspector()
{
    m_PopupEntity.Inspect("Popup Entity");
    m_PopupButton.Inspect("Popup Button");
    
    ImGui::InputText("Event Name", &m_EventName);
}


//-----------------------------------------------------------------------------
// private: reading
//-----------------------------------------------------------------------------

/// @brief  reads the PopupEntity from a JSON file
/// @param  data    the JSON file to read from
void Popup::ReadPopupEntity(nlohmann::ordered_json const& data)
{
    Stream::Read(m_PopupEntity, data);
}

/// @brief  reads the PopupButton from a JSON file
/// @param data    the JSON file to read from
void Popup::ReadPopupButton(nlohmann::ordered_json const& data)
{
    Stream::Read(m_PopupButton, data);
}

/// @brief  reads the EventName from a JSON file
/// @param data    the JSON file to read from
void Popup::ReadEventName(nlohmann::ordered_json const& data)
{
    Stream::Read(m_EventName, data);
}

//-----------------------------------------------------------------------------
// public: reading / writing
//-----------------------------------------------------------------------------


/// @brief  gets the map of read methods for this Popup
/// @return the map of read methods for this Popup
ReadMethodMap< ISerializable > const& Popup::GetReadMethods() const
{
    static ReadMethodMap< Popup > const readMethods = {
        { "PopupEntity" , &Popup::ReadPopupEntity },
        { "PopupButton" , &Popup::ReadPopupButton },
        { "EventName"   , &Popup::ReadEventName   }
    };

    return (ReadMethodMap< ISerializable > const&)readMethods;
}


/// @brief  Writes all Popup data to a JSON file.
/// @return The JSON file containing the data.
nlohmann::ordered_json Popup::Write() const
{
    nlohmann::ordered_json json;

    json["PopupEntity"] = m_PopupEntity.Write();
    json["PopupButton"] = m_PopupButton.Write();
    json["EventName"]   = m_EventName;

    return json;
}


//-----------------------------------------------------------------------------
// public: copying
//-----------------------------------------------------------------------------


    /// @brief  clones this Popup
    /// @return the newly created clone of this Popup
Popup* Popup::Clone() const
{
    return new Popup(*this);
}


//-----------------------------------------------------------------------------
// private: copying
//-----------------------------------------------------------------------------


    /// @brief  copy-constructor for the Popup
    /// @param  other   the other Popup to copy
Popup::Popup(Popup const& other) :
    Behavior( other ), m_PopupEntity( other.m_PopupEntity ), 
    m_PopupButton( other.m_PopupButton ), m_EventName( other.m_EventName )
{}


//-----------------------------------------------------------------------------
