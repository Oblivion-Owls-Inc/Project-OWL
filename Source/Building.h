///--------------------------------------------------------------------------//
/// @file   Building.h
/// @brief  Building Component Class
/// 
/// @author Aidan Straker (aidan.straker@digipen.edu)
/// @date   March 2024
///
/// @copyright Copyright (c) 2024 Digipen Institute of Technology
///--------------------------------------------------------------------------//

#pragma once

//------------------------------------------------------------------------------
// Includes:
//------------------------------------------------------------------------------
#include "Component.h"
#include "ComponentReference.h"
#include "EntityReference.h"

//------------------------------------------------------------------------------
// Forward References:
//------------------------------------------------------------------------------

template < typename TileType >
class Tilemap;
class Entity;
class Transform;

class Building : public Component
{
//-----------------------------------------------------------------------------
public: // constructor / destructor
//-----------------------------------------------------------------------------

    /// @brief Constructor
    Building();

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


    /// @brief  tilemap of the placed buildings
    ComponentReference< Tilemap< Entity* > > m_Buildings;

    /// @brief Transform component of the entity this component is attached too.
    ComponentReference<Transform> m_BuildingsTransform;

    /// @brief  the player entity
    EntityReference m_BuildingsEntity = EntityReference({ &m_Buildings });


 //-----------------------------------------------------------------------------
private: // methods
 //-----------------------------------------------------------------------------


//-----------------------------------------------------------------------------
public: // inspection
//-----------------------------------------------------------------------------


    /// @brief  shows the inspector for ExampleComponent
    virtual void Inspector() override;


//-----------------------------------------------------------------------------
private: // reading
//-----------------------------------------------------------------------------

    /// @brief Reads in all the data for the building entity.
    void readBuildingEntity(nlohmann::ordered_json const& data);

//-----------------------------------------------------------------------------
public: // reading / writing
//-----------------------------------------------------------------------------


    /// @brief  gets the map of read methods for this ExampleComponent
    /// @return the map of read methods for this ExampleComponent
    virtual ReadMethodMap< ISerializable > const& GetReadMethods() const override;


    /// @brief  writes this ExampleComponent to JSON
    /// @return the JSON data of this ExampleComponent
    virtual nlohmann::ordered_json Write() const override;


 //-----------------------------------------------------------------------------
public: // copying
 //-----------------------------------------------------------------------------


    /// @brief  clones this ExampleComponent
    /// @return the newly created clone of this ExampleComponent
    virtual Building* Clone() const override;


//-----------------------------------------------------------------------------
private: // copying
//-----------------------------------------------------------------------------


    /// @brief  copy-constructor for the ExampleComponent
    /// @param  other   the other ExampleComponent to copy
    Building(Building const& other);


    // disable assignment operator
    void operator =(Building const&) = delete;


//-----------------------------------------------------------------------------
};

