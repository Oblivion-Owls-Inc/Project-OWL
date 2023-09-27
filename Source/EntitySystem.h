/// @file       EntitySystem.h
/// @author     Steve Bukowinski (steve.bukowinski@digipen.edu)
/// @brief      System that manages the storing and handling of Entities
/// @version    0.1
/// @date       2023-09-05
/// 
/// @copyright  Copyright (c) 2023 Digipen Institute of Technology

#pragma once

#include "System.h"

#include <vector>
#include "Entity.h"
#include "Stream.h"


/// @brief  Example System meant to be copy-pasted when creating new Systems
class EntitySystem : public System
{

//-----------------------------------------------------------------------------
public: // methods
//-----------------------------------------------------------------------------

    /// @brief  Adds an Entity to the EntitySystem
    /// @param  entity  the entity to add the the EntitySystem
    void AddEntity( Entity* entity );

    /// @brief  gets an Entity by name
    /// @param  entityName  the name of the Entity to get
    /// @return the found Entity (nullptr if not found)
    Entity* GetEntity( std::string const& entityName );

    /// @brief  removes an Entity from the EntitySystem
    /// @param  entity  the Entity to remove
    void RemoveEntity( Entity* entity );

    /// @brief  checks if the EntitySystem contains the given Entity (for debugging)
    /// @param  entity  the Entity to search for
    /// @return whether or not the EntitySystem has the specified Entity
    bool HasEntity( Entity* entity );

    /// @brief  loads all of the entities in a scene
    /// @param  entityData  the json object containing the entity data
    void LoadEntities( Stream entityArray );

//-----------------------------------------------------------------------------
private: // virtual override methods
//-----------------------------------------------------------------------------

    /// @brief  Gets called whenever a scene is exited
    virtual void OnSceneExit() override;

//-----------------------------------------------------------------------------
private: // reading
//-----------------------------------------------------------------------------

    /// @brief  map of the EntitySystem read methods
    static ReadMethodMap< EntitySystem > const s_ReadMethods;

    /// @brief  gets this System's read methods
    /// @return this System's read methods
    virtual ReadMethodMap< System > const& GetReadMethods() const override;

//-----------------------------------------------------------------------------
private: // member variables
//-----------------------------------------------------------------------------

    /// @brief  Container for all Entities in the Scene
    std::vector< Entity* > m_Entities;

//-----------------------------------------------------------------------------
private: // singleton stuff
//-----------------------------------------------------------------------------

    /// @brief  Constructs the EntitySystem
    EntitySystem();


    /// @brief  The singleton instance of EntitySystem
    static EntitySystem * s_Instance;

//-----------------------------------------------------------------------------
public: // singleton stuff
//-----------------------------------------------------------------------------

    /// @brief  gets the instance of EntitySystem
    /// @return the instance of the EntitySystem
    static EntitySystem * GetInstance();

    // Prevent copying
    EntitySystem( EntitySystem& other ) = delete;
    void operator=( const EntitySystem& ) = delete;

//-----------------------------------------------------------------------------
};

/// @brief  shorthand function to get the EntitySystem instance
/// @return the EntitySystem instance
__inline EntitySystem* Entities() { return EntitySystem::GetInstance(); }
