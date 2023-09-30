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

//-----------------------------------------------------------------------------
// Class: Entity
//-----------------------------------------------------------------------------
#pragma once
class Entity
{
public:
	// Constructor
	Entity();
	// Destructor
	~Entity();

	// Dynamically allocate a clone of an existing Entity.
	Entity* Clone() const;
	// Deallocate the memory associated with an Entity.
	void Free();
	// Attach a component to an Entity.
	void Add(Component* component);

    /// @brief  Initializes all components of this Entity
    /// @note   ONLY CALL THIS IF YOU KNOW WHAT YOU'RE DOING
    void InitComponents();

    /// @brief  exits all components of this Entity
    /// @note   ONLY CALL THIS IF YOU KNOW WHAT YOU'RE DOING
    void ExitComponents();

	Component* HasComponent(std::type_index m_Type);

	// Type safe method for accessing the components.
    template < typename ComponentType >
    ComponentType* GetComponent();
    
	std::map<std::type_index, Component*>& getComponents();

	template < typename ComponentType >
	std::vector< ComponentType* > GetComponentsOfType();
	
	// Flag an entity for destruction
	void Destroy();
	// Checks whether an entity has been flagged for destruction.
	bool IsDestroyed();
	// Set the entity's name.
	void SetName(const std::string& name);
	// Get the name of the entity.
	const std::string& GetName();


	static __inline ReadMethodMap< Entity > const& GetReadMethods() { return s_ReadMethods; }

private:
	// The name of the entity.
	std::string m_Name;
	// Flag to indicate the entity is dead and should be destroyed
	bool m_IsDestroyed;
	// Container for attached components
	std::map<std::type_index, Component*> m_Components;

    // Copy Constructor
    Entity(const Entity& other);

	void ReadName(Stream stream);
	void ReadComponents(Stream stream);
	void ReadArchetype(Stream stream);


	static ReadMethodMap<Entity> s_ReadMethods;

};
//------------------------------------------------------------------------------

// Type safe method for accessing the components.
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
