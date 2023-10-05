///--------------------------------------------------------------------------//
/// @file   Entity.cpp
/// @brief  Function definitions for the Entity class
/// 
/// @author Aidan Straker (aidan.straker)
/// @date   September 2023
///
/// @copyright © 2023 DigiPen (USA) Corporation.
///--------------------------------------------------------------------------//
#include "Entity.h"

#include <algorithm>		  // std::sort
#include "Component.h"		  // Type
#include <cassert>			  // assert
#include "ComponentFactory.h" // Create.
#include "AssetLibrarySystem.h"
#include "basics.h"

//------------------------------------------------------------------------------
// Public: constructor / destructor
//------------------------------------------------------------------------------

    /// @brief  default constrcutor
    Entity::Entity() :
        m_Name( "" ),
	    m_IsDestroyed( false ),
	    m_Components()
    {}

    /// @brief destructor
    Entity::~Entity()
    {
        free();
    }

//------------------------------------------------------------------------------
// Public: methods
//------------------------------------------------------------------------------

    /// @brief  Initializes all components of this Entity
    /// @note   ONLY CALL THIS IF YOU KNOW WHAT YOU'RE DOING
    void Entity::InitComponents()
    {
        for ( auto& component : m_Components )
        {
            component.second->OnInit();
        }
    }

    /// @brief  exits all components of this Entity
    /// @note   ONLY CALL THIS IF YOU KNOW WHAT YOU'RE DOING
    void Entity::ExitComponents()
    {
        for ( auto& component : m_Components )
        {
            component.second->OnExit();
        }
    }

//------------------------------------------------------------------------------
// Public: accessors
//------------------------------------------------------------------------------

    /// @brief  attaches a component to this Entity
    /// @param  component   the component to attach to this Entity
    void Entity::AddComponent(Component* component)
    {

        // Check if the component already exists.
        assert( m_Components.find( component->GetType() ) == m_Components.end() );

        // Set the component's parent as this entity
        component->SetParent( this );

        // add it to the entity.
        m_Components[ component->GetType() ] = component;
    }

//------------------------------------------------------------------------------
// private: methods
//------------------------------------------------------------------------------

    /// @brief Deallocates all memory associated with an entity.
    void Entity::free()
    {
        // Traverse the component list
        for (auto& component : m_Components)
        {
            // Make sure the component is valid.
            assert(component.second);

            // Delete the component.
            delete component.second;
        }
        // Clear the component list.
        m_Components.clear();
    }

//------------------------------------------------------------------------------
// private: reading
//------------------------------------------------------------------------------

    /// @brief		  Clone this entity from an archetype.
    /// @param stream The json value to read from.
    void Entity::readArchetype(Stream stream)
    {
        *this = *AssetLibrary<Entity>()->GetAsset( stream.Read<std::string>() );
    }

    /// @brief		  Read in the name of entity.
    /// @param stream The json value to read from.
    void Entity::readName(Stream stream)
    {
	    m_Name = stream.Read<std::string>();
    }

    /// @brief		  Read in the data for all the components of entity.
    /// @param stream The json object to read from.
    void Entity::readComponents(Stream stream)
    {
	    for ( auto& componentData : stream.GetObject() )
	    {

            // [] operator finds the key in the map, or creates it if it doesn't exist yet.
            Component*& component = m_Components[ ComponentFactory::GetTypeId( componentData.name.GetString() ) ];

            // if the component doesn't exist yet, create and add it.
            if ( component == nullptr )
            {
                component = ComponentFactory::Create( componentData.name.GetString() );
                component->SetParent( this );
            }

            // read the component data
		    try
		    {
			    // Read in all the data for the component from the json.
			    Stream( componentData.value ).Read( component );
		    }
		    catch (std::runtime_error error)
		    {
			    std::cerr << error.what() << std::endl;
		    }
	    }
    }

    /// @brief A map of the all read methods used by the Entity class.
    ReadMethodMap< Entity > const Entity::s_ReadMethods = {
	    { "Archetype"  , &readArchetype  },
	    { "Components" , &readComponents },
	    { "Name"       , &readName	     }
    };

//------------------------------------------------------------------------------
// copying
//------------------------------------------------------------------------------

    /// @brief  copies all of another Entity's data and Components into this Entity
    /// @param  other   the entity to copy from
    void Entity::operator =( Entity const& other )
    {
        assert( m_Components.empty() );

        m_Name = other.m_Name;
        m_IsDestroyed = false;

        for ( auto component : other.m_Components )
        {
            AddComponent( component.second->Clone() );
        }
    }

//------------------------------------------------------------------------------
