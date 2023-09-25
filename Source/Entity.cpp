/******************************************************************************
filename    Entity.cpp
author      Aidan Straker
DP email    aidan.straker@digipen.edu
course      GAM200
section     A
Project		Dig Deeper

Brief Description:
  Function declerations for the entity class.

******************************************************************************/

#include "Entity.h"
#include <algorithm>   // std::sort
#include "Component.h" // Type
#include <cassert>	   // assert
#include "ComponentFactory.h" // Create.
#include "basics.h"

//------------------------------------------------------------------------------
// Public Functions:
//------------------------------------------------------------------------------

/// @brief Default constrcutor for the entity class.
Entity::Entity()
	: mName("")
	, mIsDestroyed(false)
	, components()
{
}

/// @brief  Copy constructor for the entity class
/// @param  other - the entity to be copied.
Entity::Entity(const Entity& other)
	: mName(other.mName)
	, mIsDestroyed(other.mIsDestroyed)
{
	for (auto& component : other.components)
	{
		Component* clone = component.second->Clone();
		Add(clone);
	}
}

/// @brief Destructor for the entity class.
Entity::~Entity() { Free(); }

/// @brief  Clones an entity.
/// @return A clone of an entity.
Entity* Entity::Clone() const { return new Entity(*this); }

/// @brief Deallocates all memory associated with an entity.
void Entity::Free()
{
	// Traverse the component list
	for (auto& component : components)
	{
		// Make sure the component is valid.
		assert(component.second);
		
		// Delete the component.
		delete component.second;
	}
	// Clear the component list.
	components.clear();
}

/// @brief Adds a component to an entity.
/// @param component - the component to be added.
void Entity::Add(Component* component)
{
	// Make sure the parameter is valid
	if (component)
	{
	// Set the component's parent as this entity
		component->Parent(this);
	// Check if the component already exists.
		assert( components.find( component->Type() ) == components.end() );
	// If it does not, add it to the entity.
		components[component->Type()] = component;
	}
}

Component* Entity::HasComponent(std::type_index type)
{
	// Check if the component exists.
	if (components.find(type) != components.end())
	{
		// If it does, return it.
			return components[type];
	}
	// Otherwise, return null.
	return nullptr;
}

std::map<std::type_index, Component*>& Entity::getComponents()
{
	return components;
}

/// @brief Flag an entity for destruction.
void Entity::Destroy() { mIsDestroyed = true; }

/// @brief  Checks if an entity has been destroyed or not.
/// @return If the entity has been destroyed (bool).
bool Entity::Destroyed() { return mIsDestroyed; }

/// @brief Set the entity's name.
/// @param name - name of the entity.
void Entity::SetName(const std::string& name)
{
	// Make sure the name is valid.
	if (!name.empty())
	{
		mName = name;
	}
}

/// @brief Get the name of the entity.
/// @return The name of the entity (string).
const std::string& Entity::GetName() { return mName; }

/// @brief Compares an entity's name.
/// @param name - the name to compare against.
/// @return does the name match (bool).
bool Entity::IsNamed(const std::string& name)
{
	if (!name.empty())
	{
		return (mName == name);
	}
	return false;
}

/// @brief Clone this entity from an archetype.
/// @param stream the json value to read from.
void Entity::ReadArchetype(Stream stream)
{
	/// TODO: Write this function.
}

/// @brief Read in the name of entity.
/// @param stream the json value to read from.
void Entity::ReadName(Stream stream)
{
	mName = stream.Read<std::string>();
}

/// @brief Read in the data for all the components of entity.
/// @param stream the json object to read from.
void Entity::ReadComponents( Stream stream )
{
	for ( auto& componentData : stream.GetObject() )
	{
		Component* component = ComponentFactory::Create( componentData.name.GetString() );
		try
		{
			Stream( componentData.value ).Read( component );
		}
		catch ( std::runtime_error error )
		{
			std::cerr << error.what() << std::endl;
		}
		Add( component );
	}
}

ReadMethodMap< Entity > Entity::readMethods = {
	{ "Archetype",   &ReadArchetype     },
	{ "Components",  &ReadComponents    },
	{ "Name",        &ReadName          }
};