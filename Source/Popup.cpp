/// @file       Popup.cpp
/// @author     Steve Bukowinski (steve.bukowinski@digipen.edu)
/// @brief      DESCRIPTION HERE
/// @version    0.1
/// @date       DATE HERE
/// 
/// @copyright  Copyright (c) 2024 Digipen Institute of Technology


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


//-----------------------------------------------------------------------------
// public: accessors
//-----------------------------------------------------------------------------


//-----------------------------------------------------------------------------
// public: virtual override methods
//-----------------------------------------------------------------------------


/// @brief  called once when entering the scene
void Popup::OnInit()
{
    m_PopupEntity.SetOwnerName(GetName());
    m_PopupButton.SetOwnerName(GetName());

    m_PopupEntity.Init();
    m_PopupButton.Init();

    BehaviorSystem< Popup >::GetInstance()->AddComponent(this);
    
}

/// @brief  called every frame
void Popup::OnFixedUpdate()
{
    if (m_PopupButton->GetReleased())
    {
        Entity* entity = Entities()->GetEntity(m_PopupEntity->GetName());

        if (entity != nullptr)
        {
            entity->Destroy();
        }
        else
        {
            m_PopupEntity->Clone()->AddToScene();
        }
    }
}

/// @brief  called once when exiting the scene
void Popup::OnExit()
{

}

//-----------------------------------------------------------------------------
// private: methods
//-----------------------------------------------------------------------------


//-----------------------------------------------------------------------------
// public: inspection
//-----------------------------------------------------------------------------


    /// @brief  shows the inspector for Popup
void Popup::Inspector()
{
    m_PopupEntity.Inspect("Popup Entity");
    m_PopupButton.Inspect("Popup Button");
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
//-----------------------------------------------------------------------------
// public: reading / writing
//-----------------------------------------------------------------------------


/// @brief  gets the map of read methods for this Popup
/// @return the map of read methods for this Popup
ReadMethodMap< ISerializable > const& Popup::GetReadMethods() const
{
    static ReadMethodMap< Popup > const readMethods = {
        { "PopupEntity" , &Popup::ReadPopupEntity },
        { "PopupButton" , &Popup::ReadPopupButton }
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
    Behavior( other ), m_PopupEntity( other.m_PopupEntity ), m_PopupButton( other.m_PopupButton )
{}


//-----------------------------------------------------------------------------
