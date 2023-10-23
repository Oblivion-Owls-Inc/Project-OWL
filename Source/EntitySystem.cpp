/// @file       EntitySystem.cpp
/// @author     Steve Bukowinski (steve.bukowinski@digipen.edu)
/// @brief      System that manages the storing and handling of Entities
/// @version    0.1
/// @date       2023-09-05
/// 
/// @copyright  Copyright (c) 2023 Digipen Institute of Technology

#include "EntitySystem.h"
#include "DebugSystem.h"
#include <algorithm>

///-----------------------------------------------------------------------------
/// Set Static Variables
///-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
// public methods
//-----------------------------------------------------------------------------

    /// @brief  Adds an Entity to the EntitySystem
    /// @param  entity  the entity to add the the EntitySystem
    void EntitySystem::AddEntity( Entity* entity )
    {
        m_Entities.push_back( entity );
        entity->InitComponents();
    }

    /// @brief  gets an Entity by name
    /// @param  entityName  the name of the Entity to get
    /// @return the found Entity (nullptr if not found)
    Entity* EntitySystem::GetEntity( std::string const& entityName )
    {
        auto iterator = std::find_if(
            m_Entities.begin(),
            m_Entities.end(),
            [ entityName ]( Entity* entity ) -> bool
            {
                return entity->GetName() == entityName;
            }
        );

        if ( iterator == m_Entities.end() )
        {
            return nullptr;
        }

        return *iterator;
    }

    /// @brief  removes an Entity from the EntitySystem
    /// @param  entity  the Entity to remove
    void EntitySystem::RemoveEntity( Entity* entity )
    {
        auto iterator = std::find( m_Entities.begin(), m_Entities.end(), entity );
        
        if ( iterator != m_Entities.end() )
        {
            m_Entities.erase( iterator );
            entity->ExitComponents();
        }
        else
        {
            std::ostringstream errorMessage;
            errorMessage <<
                "Error: Could not find entity \"" << entity->GetName() <<
                "\" to remove from the EntitySystem";
            throw std::runtime_error( errorMessage.str() );
        }

    }

    /// @brief  checks if the EntitySystem contains the given Entity (for debugging)
    /// @param  entity  the Entity to search for
    /// @return whether or not the EntitySystem has the specified Entity
    bool EntitySystem::HasEntity( Entity* entity )
    {
        auto iterator = std::find( m_Entities.begin(), m_Entities.end(), entity );

        return iterator != m_Entities.end();
    }

    /// @brief  loads all of the m_Entities in a scene
    /// @param  entityData  the json object containing the entity data
    void EntitySystem::LoadEntities( nlohmann::ordered_json const& data )
    {
        for ( auto& [ key, value ] : data.items() )
        {
            Entity * entity = new Entity();
            Stream::Read( entity, value );
            m_Entities.push_back( entity );
        }

        for ( Entity* entity : m_Entities )
        {
            entity->InitComponents();
        }
    }

    /// @brief  saves all of the entities in a scene
    /// @return the written json data
    nlohmann::ordered_json EntitySystem::SaveEntities() const
    {
        nlohmann::ordered_json json;

        for ( Entity* entity : m_Entities )
        {
            json[ entity->GetName() ] = entity->Write();
        }

        return json;
    }

//-----------------------------------------------------------------------------
// virtual override methods
//-----------------------------------------------------------------------------

    /// @brief  Gets called whenever a scene is exited
    void EntitySystem::OnSceneExit()
    {
        for ( Entity* entity : m_Entities )
        {
            delete entity;
        }

        m_Entities.clear();
    }

    void EntitySystem::OnFixedUpdate()
    {
        for ( int i = 0; i < m_Entities.size(); ++i )
        {
            Entity * entity = m_Entities[i];

            if (entity->IsDestroyed())
            {
				RemoveEntity(entity);
                delete entity;
                --i;
            }
		}
    }

    void EntitySystem::DebugWindow()
    {
        if (ImGui::Button(m_ShowEntityList ? "Hide Entity List" : "Show Entity List"))
            m_ShowEntityList = !m_ShowEntityList;

       if (ImGui::Button("Create Entity"))
		   m_ShowEntityCreate = !m_ShowEntityCreate;

       if (m_ShowEntityList)
		   EntityListWindow();


    }

//-----------------------------------------------------------------------------
// private: methods
//-----------------------------------------------------------------------------

    void EntitySystem::EntityListWindow()
    {
        ImGui::Begin("Entity List");

        if (!ImGui::TreeNode("Entities"))
        {
            ImGui::End();
            return;
        }

        for (const auto& entity : EntitySystem::GetInstance()->GetEntities())
        {
            std::string Name = entity->GetName() + std::string("##");
            if (!ImGui::TreeNodeEx( ( Name + std::to_string( entity->GetId() ) ).c_str() ) )
            {
                continue;
            }

            entity->Inspect();

            ImGui::TreePop();
        }

        ImGui::TreePop();
        ImGui::End();
    }

//-----------------------------------------------------------------------------
// singleton stuff
//-----------------------------------------------------------------------------


    /// @brief  Constructs the EntitySystem
    EntitySystem::EntitySystem() :
        System( "EntitySystem" ),
        m_Entities()
    {}

    /// @brief  The singleton instance of EntitySystem
    EntitySystem * EntitySystem::s_Instance = nullptr;

    /// @brief  gets the instance of EntitySystem
    /// @return the instance of the EntitySystem
    EntitySystem * EntitySystem::GetInstance()
    {
        if ( s_Instance == nullptr )
        {
            s_Instance = new EntitySystem();
        }
        return s_Instance;
    }

//-----------------------------------------------------------------------------
