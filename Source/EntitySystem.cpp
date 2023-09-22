/// @file EntitySystem.cpp
/// @author Steve Bukowinski (steve.bukowinski@digipen.edu)
/// @brief System that manages the storing and handling of Entities
/// @version 0.1
/// @date 2023-09-05
/// 
/// @copyright Copyright (c) 2023

#include "EntitySystem.h"

#include <algorithm>

//-----------------------------------------------------------------------------
// public methods
//-----------------------------------------------------------------------------

    /// @brief Adds an Entity to the EntitySystem
    /// @param entity the entity to add the the EntitySystem
    void EntitySystem::AddEntity( Entity* entity )
    {
        entities.push_back( entity );
    }

    /// @brief gets an Entity by name
    /// @param entityName the name of the Entity to get
    /// @return the found Entity (nullptr if not found)
    Entity* EntitySystem::GetEntity( std::string const& entityName )
    {
        auto iterator = std::find_if(
            entities.begin(),
            entities.end(),
            [ entityName ]( Entity* entity ) -> bool
            {
                return entity->GetName() == entityName;
            }
        );

        if ( iterator == entities.end() )
        {
            return nullptr;
        }

        return *iterator;
    }

    /// @brief removes an Entity from the EntitySystem
    /// @param entity the Entity to remove
    void EntitySystem::RemoveEntity( Entity* entity )
    {
        auto iterator = std::find( entities.begin(), entities.end(), entity );
        
        if ( iterator != entities.end() )
        {
            entities.erase( iterator );
        }
    }

    /// @brief checks if the EntitySystem contains the given Entity (for debugging)
    /// @param entity the Entity to search for
    /// @return whether or not the EntitySystem has the specified Entity
    bool EntitySystem::HasEntity( Entity* entity )
    {
        auto iterator = std::find( entities.begin(), entities.end(), entity );

        return iterator != entities.end();
    }

//-----------------------------------------------------------------------------
// private methods
//-----------------------------------------------------------------------------


//-----------------------------------------------------------------------------
// virtual override methods
//-----------------------------------------------------------------------------

    /// @brief Gets called whenever a scene is exited
    void EntitySystem::OnSceneExit()
    {
        for ( Entity* entity : entities )
        {
            delete entity;
        }

        entities.clear();
    }

//-----------------------------------------------------------------------------
// singleton stuff
//-----------------------------------------------------------------------------

/// @brief Constructs the EntitySystem
EntitySystem::EntitySystem() {}

/// @brief The singleton instance of EntitySystem
EntitySystem * EntitySystem::instance = nullptr;

/// @brief gets the instance of EntitySystem
/// @return the instance of the EntitySystem
EntitySystem * EntitySystem::getInstance()
{
    if ( instance == nullptr )
    {
        instance = new EntitySystem();
    }
    return instance;
}

//-----------------------------------------------------------------------------
