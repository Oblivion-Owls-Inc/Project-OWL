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

#include "ISerializable.h"

//-----------------------------------------------------------------------------
// Class: Entity
//-----------------------------------------------------------------------------
class Entity : public ISerializable
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
    __inline void Destroy() { m_IsDestroyed = true; }

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

    /// @brief  gets the component of the specified type from this Entity
    /// @tparam ComponentType   the type of component to get
    /// @return the component of the specified type (nullptr if component doesn't exist)
    template < typename ComponentType >
    ComponentType const* GetComponent() const;

    /// @brief  gets all of the components derived from the specified type from this Entity
    /// @tparam ComponentType   the type of component to get
    /// @return a vector of all components of the specified type
    template < typename ComponentType >
    std::vector< ComponentType* > GetComponentsOfType();

    /// @brief  gets all components in this Entity
    /// @return the map of all components in this Entity
    __inline std::map< std::type_index, Component* >& getComponents() { return m_Components; }

    /// @brief  gets whether this Entity is flagged for destruction
    /// @return whether this Entity is flagged for destruction
    __inline bool IsDestroyed() const { return m_IsDestroyed; }

    /// @brief  sets this Entity's name
    /// @param  name    the new name for this Entity
    __inline void SetName( std::string const& name ) { m_Name = name; }

    /// @brief  gets this Entity's name
    /// @return this Entity's name
    __inline std::string const& GetName() const { return m_Name; }

     /// @brief used by the Debug System to display information about this Entity
     void Inspect();


     /// @brief  gets the Id of this Component
     /// @return the Id of this Component
     unsigned GetId() const { return m_Id; }

//-----------------------------------------------------------------------------
private: // methods
//-----------------------------------------------------------------------------

	/// @brief  deletes all components used by this Entity
	void free();

    void AddComponent();

    void RemoveComponent();

    void RenameEntity();

//-----------------------------------------------------------------------------
private: // reading
//-----------------------------------------------------------------------------


    /// @brief  reads this Entity's Archetype
    /// @param  stream  the json data to read from
    void readArchetype( nlohmann::ordered_json const& data );

    /// @brief  reads this Entity's Name
    /// @param  stream  the json data to read from
    void readName( nlohmann::ordered_json const& data );

    /// @brief  reads this Entity's Components
    /// @param  stream  the json data to read from
    void readComponents( nlohmann::ordered_json const& data );

    /// @brief  map of read methods for Entity
    static ReadMethodMap< Entity > const s_ReadMethods;

    /// @brief  gets the map of read methods for Entities
    /// @return the map of read methods for Entities
    virtual ReadMethodMap< ISerializable > const& GetReadMethods() const override
    {
        return (ReadMethodMap< ISerializable > const&)s_ReadMethods;
    }

public:

    /// @brief  Write all Entity data to a JSON file.
    /// @return The JSON file containing the Entity data.
    virtual nlohmann::ordered_json Write() const override;

//-----------------------------------------------------------------------------
private: // member variables
//-----------------------------------------------------------------------------
	
	/// @brief  this Entity's name
	std::string m_Name;

	/// @brief  flag of whether this Entity should be destroyed
	bool m_IsDestroyed;

	/// @brief containter of components attached to this Entity
	std::map<std::type_index, Component*> m_Components;

    bool m_AddComponent = false;

    bool m_RemoveComponent = false;

    bool m_RenameEntity = false;

    /// @brief  the ID of this Component
    unsigned m_Id;
//-----------------------------------------------------------------------------
public: // copying
//-----------------------------------------------------------------------------

    /// @brief  copies all of another Entity's data and Components into this Entity
    /// @param  other   the entity to copy from
    void operator =( Entity const& other );

    /// @brief  makes a copy of this Entity
    /// @return the new copy of this Entity
    Entity* Clone() const {
        Entity* clone = new Entity;
        *clone = *this;
        return clone;
    }

    // prevent non-assignment copying
    Entity( Entity const& other ) = delete;


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
        if ( componentIterator != m_Components.end() )
        {
            return static_cast< ComponentType* >((*componentIterator).second);
        }

        // if exact type not found, fall back to searching for a derived type
        for ( auto pair : m_Components )
        {
            ComponentType* component = dynamic_cast< ComponentType* >( pair.second );
            if ( component != nullptr )
            {
                return component;
            }
        }

        // if no derived component found, return nullptr
        return nullptr;
    }

    /// @brief  gets the component of the specified type from this Entity
    /// @tparam ComponentType   the type of component to get
    /// @return the component of the specified type (nullptr if component doesn't exist)
    template < typename ComponentType >
    ComponentType const* Entity::GetComponent() const
    {
        return const_cast< Entity* >( this )->GetComponent< ComponentType >();
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

//-----------------------------------------------------------------------------