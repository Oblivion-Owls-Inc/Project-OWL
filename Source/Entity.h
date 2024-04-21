//-----------------------------------------------------------------------------
/// @file   Entity.h
/// @brief  Entity class which holds Components
/// 
/// @author Aidan Straker (aidan.straker), Steve Bukowinski (steve.bukowinski)
/// @date   September 2023
///
/// @copyright (c) 2024 DigiPen (USA) Corporation.
//-----------------------------------------------------------------------------

#pragma once
#define ENTITY_H

//-----------------------------------------------------------------------------
// Include Files:
//-----------------------------------------------------------------------------
#include "pch.h" 
#include "Component.h"
#include "ISerializable.h"

class EntityReference;
class ComponentReferenceBase;

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


    /// @brief  flags this Entity for destruction
    void Destroy();


    /// @brief  attaches a component to this Entity
    /// @param  component   the component to attach to this Entity
    void AddComponent( Component* component );


    /// @brief  gets the component of the specified type from this Entity
    /// @tparam ComponentType   the type of component to get
    /// @return the component of the specified type (nullptr if component doesn't exist)
    template < typename ComponentType >
    ComponentType const* GetComponent() const;

    /// @brief  gets the component of the specified type from this Entity
    /// @tparam ComponentType   the type of component to get
    /// @return the component of the specified type (nullptr if component doesn't exist)
    template < typename ComponentType >
    ComponentType* GetComponent();


    /// @brief  gets all of the components derived from the specified type from this Entity
    /// @tparam ComponentType   the type of component to get
    /// @return a vector of all components of the specified type
    template < typename ComponentType >
    std::vector< ComponentType* > GetComponentsOfType();


    /// @brief  sets the parent of this Entity
    /// @param  parent  the Entity that should be the parent of this one
    void SetParent( Entity* parent );


    /// @brief  checks if this Entity is descended from another Entity
    /// @param  ancestor    The entity to check if this Entity is descended from
    /// @return whether this Entity is descended from the other Entity
    bool IsDescendedFrom( Entity const* ancestor ) const;


    /// @brief  queues this Entity and its children to be added to the Scene
    void AddToScene();


//-----------------------------------------------------------------------------
public: // accessors
//-----------------------------------------------------------------------------


    /// @brief  gets all components in this Entity
    /// @return the map of all components in this Entity
    std::map< std::type_index, Component* >& getComponents();


    /// @brief  gets whether this Entity is flagged for destruction
    /// @return whether this Entity is flagged for destruction
    bool IsDestroyed() const;


    /// @brief  gets this Entity's name
    /// @return this Entity's name
    std::string const& GetName() const;

    /// @brief  sets this Entity's name
    /// @param  name    the new name for this Entity
    void SetName( std::string const& name );


    /// @brief  gets the Id of this Component
    /// @return the Id of this Component
    unsigned GetId() const;


    /// @brief  gets the parent of this Entity
    /// @return the parent of this Entity
    Entity const* GetParent() const;

    /// @brief  gets the parent of this Entity
    /// @return the parent of this Entity
    Entity* GetParent();


    /// @brief  gets the children of this Entity
    /// @return the children of this Entity
    std::vector< Entity const* > const& GetChildren() const;

    /// @brief  gets the children of this Entity
    /// @return the children of this Entity
    std::vector< Entity* > const& GetChildren();


    /// @brief  gets the number of descendants this Entity has
    /// @return the number of descendants this Entity has
    int GetNumDescendants() const;


    /// @brief  gets whether the Entity is currently in the scene
    /// @return whether the Entity is currently in the scene
    bool IsInScene() const;


//-----------------------------------------------------------------------------
private: // member variables
//-----------------------------------------------------------------------------


    /// @brief  this Entity's name
    std::string m_Name = "";

    /// @brief  containter of components attached to this Entity
    std::map< std::type_index, Component* > m_Components = {};

    /// @brief  the ID of this Component
    unsigned m_Id = -1;


    /// @brief  the children of this Entity
    std::vector< Entity* > m_Children = {};

    /// @brief  the number of descendants this Entity has
    int m_NumDescendants = 0;


    /// @brief  the parent Entity of this Entity
    Entity* m_Parent = nullptr;


    /// @brief  flag of whether this Entity should be destroyed
    bool m_IsDestroyed = false;


    /// @brief  whether the Entity is currently in the scene
    bool m_IsInScene = false;


    /// @brief  whether the Parent needs to be set on Init
    bool m_SetParentOnInit = false;


    /// @brief  all of the ComponentReferences currently tracking this Entity
    std::set< ComponentReferenceBase* > m_ComponentReferences;

    /// @brief  all of the EntityReferences currently tracking this Entity
    std::set< EntityReference* > m_EntityReferences;

//-----------------------------------------------------------------------------
public: // engine methods
//-----------------------------------------------------------------------------


    /// @brief  Initializes all components / children of this Entity
    /// @brief  FOR ENGINE USE ONLY - only call this if you're modifying core engine functionality
    void Init();

    /// @brief  exits all components / children of this Entity
    /// @brief  FOR ENGINE USE ONLY - only call this if you're modifying core engine functionality
    void Exit();


    /// @brief  adds an EntityReference to this Entity
    /// @param  entityReference the refernce to add
    void AddEntityReference( EntityReference* entityReference );

    /// @brief  removes an EntityReference to this Entity
    /// @param  entityReference the refernce to remove
    void RemoveEntityReference( EntityReference* entityReference );


    /// @brief  adds an ComponentReference to this Component
    /// @param  componentReference  the refernce to add
    void AddComponentReference( ComponentReferenceBase* componentReference ) noexcept;

    /// @brief  removes an ComponentReference to this Component
    /// @param  componentReference  the refernce to remove
    void RemoveComponentReference( ComponentReferenceBase* componentReference ) noexcept;


//-----------------------------------------------------------------------------
private: // methods
//-----------------------------------------------------------------------------
    
    
    /// @brief  adds a child to this Enity
    /// @param  child   - the child to add to this Enitity
    void addChild( Entity* child );

    /// @brief  removes a child from this Enity
    /// @param  child   - the child to remove from this Enitity
    void removeChild( Entity* child );


    /// @brief  changes descendant count and propagates that change
    /// @param  changeBy    the number of descendants added or removed
    void propagateDescendantChange( int changeBy );


    /// @brief  propagates an OnHeirarchyChange event downwards
    /// @param  previousParent the previous parent of this Entity
    void propagateHeirachyChangeEvent( Entity* previousParent );


//-----------------------------------------------------------------------------
public: // inspection
//-----------------------------------------------------------------------------


    /// @brief used by the Debug System to display information about this Entity
    void Inspect();


    /// @brief used by the Debug System to Rename this Entity
    void RenameEntity( char const* popup_id );

    
//-----------------------------------------------------------------------------
private: // inspection
//-----------------------------------------------------------------------------


    /// @brief  removes a Component using the Inspector
    /// @param  component   the Component to remove
    void removeComponent( Component* component );


//-----------------------------------------------------------------------------
private: // reading
//-----------------------------------------------------------------------------


    /// @brief  reads this Entity's Archetype
    /// @param  data    the json data to read from
    void readArchetype( nlohmann::ordered_json const& data );

    /// @brief  reads this Entity's Name
    /// @param  data    the json data to read from
    void readName( nlohmann::ordered_json const& data );


    /// @brief  reads this Entity's Components
    /// @param  data    the json data to read from
    void readComponents( nlohmann::ordered_json const& data );


    /// @brief  reads the Children of this Entity
    /// @param  data    the json data to read from
    void readChildren( nlohmann::ordered_json const& data );


//-----------------------------------------------------------------------------
public: // reading / writing
//-----------------------------------------------------------------------------


    /// @brief  gets the map of read methods for Entities
    /// @return the map of read methods for Entities
    virtual ReadMethodMap< ISerializable > const& GetReadMethods() const override;


    /// @brief  Write all Entity data to a JSON file.
    /// @return The JSON file containing the Entity data.
    virtual nlohmann::ordered_json Write() const override;


//-----------------------------------------------------------------------------
public: // copying
//-----------------------------------------------------------------------------


    /// @brief  makes a copy of this Entity
    /// @return the new copy of this Entity
    Entity* Clone() const
    {
        Entity* clone = new Entity;
        *clone = *this;
        return clone;
    }


    /// @brief  copies all of another Entity's data and Components into this Entity
    /// @param  other   the entity to copy from
    void operator =( Entity const& other );


    // prevent non-assignment copying
    Entity( Entity const& other ) = delete;


//------------------------------------------------------------------------------
};

//------------------------------------------------------------------------------
// template implementation
//------------------------------------------------------------------------------


    /// @brief  gets the component of the specified type from this Entity
    /// @tparam ComponentType   the type of component to get
    /// @return the component of the specified type (nullptr if component doesn't exist)
    template < typename ComponentType >
    ComponentType const* Entity::GetComponent() const
    {
        auto componentIterator = m_Components.find( typeid( ComponentType ) );
        if ( componentIterator != m_Components.end() )
        {
            return static_cast< ComponentType* >( componentIterator->second );
        }

        // if exact type not found, fall back to searching for a derived type
        for ( auto const& [ type, component ] : m_Components )
        {
            ComponentType const* found = dynamic_cast< ComponentType const* >( component );
            if ( found != nullptr )
            {
                return found;
            }
        }

        // if no derived component found, return nullptr
        return nullptr;
    }


    /// @brief  gets the component of the specified type from this Entity
    /// @tparam ComponentType   the type of component to get
    /// @return the component of the specified type (nullptr if component doesn't exist)
    template < typename ComponentType >
    ComponentType* Entity::GetComponent()
    {
        return const_cast< ComponentType* >( const_cast< Entity const* >( this )->GetComponent< ComponentType >() );
    }


    /// @brief  gets all of the components derived from the specified type from this Entity
    /// @tparam ComponentType   the type of component to get
    /// @return a vector of all components of the specified type
    template < typename ComponentType >
    std::vector< ComponentType* > Entity::GetComponentsOfType()
    {
	    std::vector< ComponentType* > componentsOfType;

	    for ( auto& [ type, component ] : m_Components ) 
	    {
            ComponentType* casted = dynamic_cast< ComponentType* >( component );
		    if ( casted != nullptr )
		    {
			    componentsOfType.push_back( casted );
		    }
	    }

        return componentsOfType;
    }


//-----------------------------------------------------------------------------