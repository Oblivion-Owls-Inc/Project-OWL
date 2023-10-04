///--------------------------------------------------------------------------//
/// @file   Entity.h
/// @brief  Declaration for the Entity Class
/// 
/// @author Aidan Straker (aidan.straker)
/// @date   September 2023
///
/// @copyright © 2023 DigiPen (USA) Corporation.
///--------------------------------------------------------------------------//    
#pragma once

//-----------------------------------------------------------------------------
// Include Files:
//-----------------------------------------------------------------------------

#include "Component.h"
#include <map>        // std::map
#include <string>	  // std::string
#include <vector>	  // std::vector
#include "Stream.h"

#pragma once

//-----------------------------------------------------------------------------
// Class: Entity
//-----------------------------------------------------------------------------
class Entity
{
//-----------------------------------------------------------------------------
public: // constructor / destructor
//-----------------------------------------------------------------------------

	/// @brief default constructor
	Entity();

	/// @brief destructor
	~Entity();

//-----------------------------------------------------------------------------
public: // methods
//-----------------------------------------------------------------------------

    /// @brief  Initializes all components of this Entity
    /// @note   ONLY CALL THIS IF YOU KNOW WHAT YOU'RE DOING
    void InitComponents();

    /// @brief  exits all components of this Entity
    /// @note   ONLY CALL THIS IF YOU KNOW WHAT YOU'RE DOING
    void ExitComponents();

    /// @brief  flags this Entity for destruction
    void Destroy();

//-----------------------------------------------------------------------------
public: // accessors
//-----------------------------------------------------------------------------

    /// @brief  attaches a component to this Entity
    /// @param  component   the component to attach to this Entity
    void AddComponent( Component* component );

    /// @brief  gets the component of the specified type from this Entity
    /// @tparam ComponentType   the type of component to get
    /// @return the component of the specified type (nullptr if component doesn't exist)
    template < typename ComponentType >
    ComponentType* GetComponent();

    /// @brief  gets all components in this Entity
    /// @return the map of all components in this Entity
    std::map< std::type_index, Component* >& getComponents();

    /// @brief  gets all of the components derived from the specified type from this Entity
    /// @tparam ComponentType   the type of component to get
    /// @return a vector of all components of the specified type
    template < typename ComponentType >
    std::vector< ComponentType* > GetComponentsOfType();

    /// @brief  gets whether this Entity is flagged for destruction
    /// @return whether this Entity is flagged for destruction
    bool IsDestroyed() const;

    /// @brief  sets this Entity's name
    /// @param  name    the new name for this Entity
    void SetName( std::string const& name );

    /// @brief  gets this Entity's name
    /// @return this Entity's name
    const std::string& GetName() const;

//-----------------------------------------------------------------------------
private: // methods
//-----------------------------------------------------------------------------

	/// @brief  deletes all components used by this Entity
	void free();

//-----------------------------------------------------------------------------
public: // reading
//-----------------------------------------------------------------------------

	/// @brief  gets the map of read methods for Entities
	/// @return the map of read methods for Entities
	static __inline ReadMethodMap< Entity > const& GetReadMethods() { return s_ReadMethods; }

//-----------------------------------------------------------------------------
private: // reading
//-----------------------------------------------------------------------------

    /// @brief  reads this Entity's Archetype
    /// @param  stream  the json data to read from
    void readArchetype( Stream stream );

    /// @brief  reads this Entity's Name
    /// @param  stream  the json data to read from
    void readName( Stream stream );

    /// @brief  reads this Entity's Components
    /// @param  stream  the json data to read from
    void readComponents( Stream stream );

    /// @brief  map of read methods for Entity
    static ReadMethodMap< Entity > const s_ReadMethods;

//-----------------------------------------------------------------------------
private: // member variables
//-----------------------------------------------------------------------------
	
	/// @brief  this Entity's name
	std::string m_Name;

	/// @brief  flag of whether this Entity should be destroyed
	bool m_IsDestroyed;

	/// @brief containter of components attached to this Entity
	std::map<std::type_index, Component*> m_Components;

//-----------------------------------------------------------------------------
public: // copying
//-----------------------------------------------------------------------------

    /// @brief  Creates a copy of this Entity and all of its Components
    /// @return the newly created copy of this Entity
    Entity* Clone() const;

//-----------------------------------------------------------------------------
private: // copying
//-----------------------------------------------------------------------------
    
    /// @brief  copy constructor
    /// @param  other   the Entity to copy
    Entity( Entity const& other );

//------------------------------------------------------------------------------
};

//------------------------------------------------------------------------------
// template methods
//------------------------------------------------------------------------------

    /// @brief  gets the component of the specified type from this Entity
    /// @tparam ComponentType   the type of component to get
    /// @return the component of the specified type (nullptr if component doesn't exist)
    template < typename ComponentType >
    ComponentType* Entity::GetComponent()
    {
        auto componentIterator = m_Components.find( typeid( ComponentType ) );
        if ( componentIterator == m_Components.end() )
        {
            return nullptr;
        }
        return static_cast<ComponentType*>((*componentIterator).second);
    }

    /// @brief  gets all of the components derived from the specified type from this Entity
    /// @tparam ComponentType   the type of component to get
    /// @return a vector of all components of the specified type
    template < typename ComponentType >
    std::vector< ComponentType* > Entity::GetComponentsOfType()
    {
	    std::vector<ComponentType*> componentsOfType;

	    for ( auto component : m_Components ) 
	    {
            ComponentType* casted = dynamic_cast<ComponentType*>( component.second );
		    if ( casted != nullptr )
		    {
			    componentsOfType.push_back( casted );
		    }
	    }

        return componentsOfType;

    }

//------------------------------------------------------------------------------
