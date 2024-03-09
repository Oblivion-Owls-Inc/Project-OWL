///--------------------------------------------------------------------------//
/// @file   Building.cpp
/// @brief  Building Component Class
/// 
/// @author Aidan Straker (aidan.straker)
/// @date   March 2024
///
/// @copyright (c) 2024 DigiPen (USA) Corporation.
///--------------------------------------------------------------------------//

#include "Building.h"
#include "Tilemap.h"
#include "Entity.h"
#include "Transform.h"
#include "ComponentReference.t.h"

//-----------------------------------------------------------------------------
// public: constructor / Destructor
//-----------------------------------------------------------------------------

    /// @brief  default constructor
    Building::Building() :
        Component(typeid(Building))
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
    void Building::OnInit()
    {
        m_BuildingsEntity.SetOwnerName(GetName());
        m_BuildingsEntity.Init();

        // Make sure both component references are initialised.
        if ( m_Buildings == nullptr && m_BuildingsTransform == nullptr)
        {
            return;
        }

        m_Buildings->SetTile(m_BuildingsTransform->GetTranslation(), m_BuildingsEntity);
    }

    /// @brief  called once when exiting the scene
    void Building::OnExit()
    {
        m_BuildingsEntity.Exit();
    }


//-----------------------------------------------------------------------------
// private: methods
//-----------------------------------------------------------------------------


//-----------------------------------------------------------------------------
// public: inspection
//-----------------------------------------------------------------------------


    /// @brief  shows the inspector for Building
    void Building::Inspector()
    {

    }


//-----------------------------------------------------------------------------
// private: reading
//-----------------------------------------------------------------------------

    /// @brief Reads in all the data for the building entity.
    void Building::readBuildingEntity(nlohmann::ordered_json const& data)
    {
        Stream::Read(m_BuildingsEntity, data);
    }

//-----------------------------------------------------------------------------
// public: reading / writing
//-----------------------------------------------------------------------------


    

    /// @brief  gets the map of read methods for this Building
    /// @return the map of read methods for this Building
    ReadMethodMap< ISerializable > const& Building::GetReadMethods() const
    {
        static ReadMethodMap< Building > const readMethods = {
            { "BuildingEntity" , &Building::readBuildingEntity }
        };

        return (ReadMethodMap< ISerializable > const&)readMethods;
    }


    /// @brief  Writes all Building data to a JSON file.
    /// @return The JSON file containing the data.
    nlohmann::ordered_json Building::Write() const
    {
        nlohmann::ordered_json json;

        json["BuildingEntity"] = m_BuildingsEntity.Write();

        return json;
    }


//-----------------------------------------------------------------------------
// public: copying
//-----------------------------------------------------------------------------


    /// @brief  clones this Building
    /// @return the newly created clone of this Building
    Building* Building::Clone() const
    {
        return new Building(*this);
    }


//-----------------------------------------------------------------------------
// private: copying
//-----------------------------------------------------------------------------


    /// @brief  copy-constructor for the Building
    /// @param  other   the other Building to copy
    Building::Building(Building const& other) :
        Component(other)
    {}


//-----------------------------------------------------------------------------
