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


class Entity;


/// @brief  Example System meant to be copy-pasted when creating new Systems
class EntitySystem : public System
{
//-----------------------------------------------------------------------------
public: // methods
//-----------------------------------------------------------------------------


    /// @brief  gets an Entity by name
    /// @param  entityName  the name of the Entity to get
    /// @return the found Entity (nullptr if not found)
    Entity* GetEntity( std::string const& entityName );

    /// @brief  returns the container of all Entities in the Scene
    /// @return the container of all Entities in the Scene
    std::vector< Entity* > const& GetEntities() const;


    /// @brief  checks if the EntitySystem contains the given Entity (for debugging)
    /// @param  entity  the Entity to search for
    /// @return whether or not the EntitySystem has the specified Entity
    bool HasEntity( Entity* entity );

    /// @brief Clears the Entity being inspected
    void ClearSelectedEntity();

///-----------------------------------------------------------------------------
public: //Accessors
///-----------------------------------------------------------------------------

 	/// @brief  returns the entity being viewed in the inspector
	/// @return the Entity being viewed in the inspector
	Entity* GetSelectedEntity() const;


//-----------------------------------------------------------------------------
private: // virtual override methods
//-----------------------------------------------------------------------------


    /// @brief  Gets called whenever a scene is exited
    virtual void OnSceneExit() override;


    /// @brief Gets Called each frame
    virtual void OnUpdate( float ) override;


//-----------------------------------------------------------------------------
private: // member variables
//-----------------------------------------------------------------------------


    /// @brief  Container for all Entities in the Scene
    std::vector< Entity* > m_Entities = {};

    /// @brief  Enities queued to be added to the EntitySystem
    std::vector< Entity* > m_EntitiesToAdd = {};


    /// @brief  should the debug window be popped out
    bool m_PopOut = false;

    /// @brief  is the Entity Create window being shown
    bool m_ShowEntityCreate = true;

    /// @brief Entity that is selected by the Inpector
    Entity* selectedEntity = nullptr; 


//-----------------------------------------------------------------------------
public: // engine methods
//-----------------------------------------------------------------------------


    /// @brief  queues an Entity to be added to the EntitySystem
    /// @brief  FOR ENGINE USE ONLY - call this only if you're modifying core engine functionality
    /// @param  entity  the entity to add the the EntitySystem
    void QueueAddEntity( Entity* entity );

    
    /// @brief  moves an Entity to the end of its parent's children
    /// @brief  FOR ENGINE USE ONLY - call this only if you're modifying core engine functionality
    /// @param  entity  - the entity to move
    void MoveEntityAfterParent( Entity* entity );

    /// @brief  moves an Entity to the end the EntitySystem
    /// @brief  FOR ENGINE USE ONLY - call this only if you're modifying core engine functionality
    /// @param  entity  - the entity to move
    void MoveToEnd( Entity* entity );


//-----------------------------------------------------------------------------
private: // methods
//-----------------------------------------------------------------------------

    /// @brief  removes all queued Entities from the EntitySystem
    void removeEntities();

    /// @brief  adds all queued Entites to the EntitySystem
    void addEntities();


    /// @brief  adds the children of a loaded entity to the entities array
    /// @param  entity  the Entity to add the children of
    void addLoadedChildren( Entity* entity );


//-----------------------------------------------------------------------------
public: // inspection
//-----------------------------------------------------------------------------


    /// @brief  shows this System's debug window
    virtual void DebugWindow() override;

    /// @brief  Shows the Entity Create Window
    /// @return whether the window should be kept open
    bool EntityCreateWindow();


//-----------------------------------------------------------------------------
private: // inspection
//-----------------------------------------------------------------------------

    /// @brief Displays the Entity Hierarchy in the Entity List window
    /// @param SelectedEntity - the entity that is currently selected 
    /// @param child - whether or not to display the children of the selected entity
    void DisplayEntityHierarchy(Entity** SelectedEntity, bool children = false);

    /// @brief Shows the Properites of the Selected Entity
    void entityPropertiesWindow(Entity* entity);

//-----------------------------------------------------------------------------
public: // reading / writing
//-----------------------------------------------------------------------------


    /// @brief  loads all of the entities in a scene
    /// @param  entityData  the json object containing the entity data
    void LoadEntities( nlohmann::ordered_json const& data );


    /// @brief  saves all of the entities in a scene
    /// @return the written json data
    nlohmann::ordered_json SaveEntities() const;


//-----------------------------------------------------------------------------
public: // singleton stuff
//-----------------------------------------------------------------------------


    /// @brief  gets the instance of EntitySystem
    /// @return the instance of the EntitySystem
    static EntitySystem * GetInstance();


//-----------------------------------------------------------------------------
private: // singleton stuff
//-----------------------------------------------------------------------------


    /// @brief  Constructs the EntitySystem
    EntitySystem();


    // Prevent copying
    EntitySystem( EntitySystem const& ) = delete;
    void operator =( EntitySystem const& ) = delete;


//-----------------------------------------------------------------------------
};

/// @brief  shorthand function to get the EntitySystem instance
/// @return the EntitySystem instance
__inline EntitySystem* Entities() { return EntitySystem::GetInstance(); }
