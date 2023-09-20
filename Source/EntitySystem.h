/// @file EntitySystem.h
/// @author Steve Bukowinski (steve.bukowinski@digipen.edu)
/// @brief System that manages the storing and handling of Entities
/// @version 0.1
/// @date 2023-09-05
/// 
/// @copyright Copyright (c) 2023

#pragma once

#include "System.h"

#include <vector>
#include "Entity.h"
#include "Stream.h"


/// @brief Example System meant to be copy-pasted when creating new Systems
class EntitySystem : public System
{

public: // methods

    /// @brief Adds an Entity to the EntitySystem
    /// @param entity the entity to add the the EntitySystem
    void AddEntity( Entity* entity );

    /// @brief gets an Entity by name
    /// @param entityName the name of the Entity to get
    /// @return the found Entity (nullptr if not found)
    Entity* GetEntity( std::string const& entityName );

    /// @brief removes an Entity from the EntitySystem
    /// @param entity the Entity to remove
    void RemoveEntity( Entity* entity );

    /// @brief checks if the EntitySystem contains the given Entity (for debugging)
    /// @param entity the Entity to search for
    /// @return whether or not the EntitySystem has the specified Entity
    bool HasEntity( Entity* entity );

    /// @brief loads all of the entities in a scene
    /// @param entityData the json object containing the entity data
    void LoadEntities( Stream entityArray );

private: // methods

private: // member variables

    /// @brief Container for all Entities in the Scene
    std::vector< Entity* > entities;

private: // virtual override methods

    /// @brief Gets called whenever a scene is exited
    virtual void OnSceneExit() override;

private: // unused virtual overrides

    /// @brief Gets called once when this System is added to the Engine
    virtual void OnInit() override {}


    /// @brief Gets called once every simulation frame. Use this function for anything that affects the simulation.
    virtual void OnFixedUpdate() override {}


    /// @brief Gets called once every graphics frame. Do not use this function for anything that affects the simulation.
    /// @param dt the elapsed time in seconds since the previous frame
    virtual void OnUpdate( float dt ) override {}


    /// @brief Gets called once before the Engine closes
    virtual void OnExit() override {}


    /// @brief Gets called whenever a new Scene is loaded
    virtual void OnSceneLoad() override {}


    /// @brief Gets called whenever a scene is initialized
    virtual void OnSceneInit() override {}


    /// @brief Loads the config data of this System
    /// @param configData the JSON object with all of the configData for this System
    virtual void Load( rapidjson::Value const& configData ) override {}

private: // singleton stuff

    /// @brief Constructs the EntitySystem
    EntitySystem();


    /// @brief The singleton instance of EntitySystem
    static EntitySystem * instance;

public: // singleton stuff

    /// @brief gets the instance of EntitySystem
    /// @return the instance of the EntitySystem
    static EntitySystem * getInstance();

    // Prevent copying
    EntitySystem(EntitySystem& other) = delete;
    void operator=(const EntitySystem&) = delete;
};

__inline EntitySystem* Entities() { return EntitySystem::getInstance(); }
