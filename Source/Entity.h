/**
 * @file   Entity.h
 * @author Aidan Straker (aidan.straker@digipen.edu)
 * @brief  Function headers for the entity class.
 * @version 0.1
 * @date 2023-09-06
 *
 * @copyright Copyright (c) 2023
 *
 */
#pragma once

//------------------------------------------------------------------------------
// Include Files:
//------------------------------------------------------------------------------

#include "Component.h"
#include <map>  // std::map
#include <string>  // std::string
#include <fstream> // std::ifstream

//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
// Forward References:
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
// Class: Entity

//------------------------------------------------------------------------------
// Class: Entity
//------------------------------------------------------------------------------
#pragma once
class Entity
{
public:
	// Constructor
	Entity();
	// Copy Constructor
	Entity(const Entity& other);
	// Destructor
	~Entity();

	// Dynamically allocate a clone of an existing Entity.
	Entity* Clone() const;
	// Deallocate the memory associated with an Entity.
	void Free();
	// Read (and construct) the components associated with a entity.
	void Read(std::ifstream& stream);
	// Attach a component to an Entity.
	void Add(Component* component);

	Component* HasComponent(std::type_index type);

	// Type safe method for accessing the components.
    template < typename ComponentType >
    ComponentType* GetComponent()
    {
        auto componentIterator = components.find( typeid( ComponentType ) );
        if ( componentIterator == components.end() )
        {
            return nullptr;
        }
        return static_cast<ComponentType*>( componentIterator->second );
    }
    
	// Flag an entity for destruction
	void Destroy();
	// Checks whether an entity has been flagged for destruction.
	bool Destroyed();
	// Set the entity's name.
	void SetName(const std::string& name);
	// Get the name of the entity.
	const std::string& GetName();
	// Compare the entity's name with the specified name.
	bool IsNamed(const std::string& name);

private:
	// The name of the entity.
	std::string mName;
	// Flag to indicate the entity is dead and should be destroyed
	bool mIsDestroyed;
	// Container for attached components
	std::map<std::type_index, Component*> components;

	// Private Member Function

	// Searches the components vector for a specific component
	// Component* BinarySearch(Component::TypeEnum type);
	// Get the a specific componetfrom an entity
	// Component* Get(Component::TypeEnum type);
};
//------------------------------------------------------------------------------

