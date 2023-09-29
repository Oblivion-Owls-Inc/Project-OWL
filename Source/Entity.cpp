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
#include "basics.h"

//------------------------------------------------------------------------------
// Public Functions:
//------------------------------------------------------------------------------

/// @brief Default constrcutor for the entity class.
Entity::Entity()
	: m_Name("")
	, m_IsDestroyed(false)
	, m_Components()
{
}

/// @brief		  Copy constructor for the entity class
/// @param other  The entity to be copied.
Entity::Entity(const Entity& other)
	: m_Name(other.m_Name)
	, m_IsDestroyed(other.m_IsDestroyed)
{
	for (auto& component : other.m_Components)
	{
		Component* clone = component.second->Clone();
		Add(clone);
	}
}

/// @brief Destructor for the entity class.
Entity::~Entity() { Free(); }

/// @brief  Clones an entity.
/// @return A clone of an entity (Entity*).
Entity* Entity::Clone() const { return new Entity(*this); }

/// @brief Deallocates all memory associated with an entity.
void Entity::Free()
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

/// @brief			 Adds a component to an entity.
/// @param component The component to be added.
void Entity::Add(Component* component)
{
	// Make sure the parameter is valid
	if (component)
	{
	// Set the component's parent as this entity
		component->Parent(this);
	// Check if the component already exists.
		assert(m_Components.find(component->Type()) == m_Components.end());
	// If it does not, add it to the entity.
		m_Components[component->Type()] = component;
	}
}

/// @brief		Checks if an entity has a specific component.
/// @param type The type ID of the component.
/// @return		The component if found (Component*).
Component* Entity::HasComponent(std::type_index type)
{
	// Check if the component exists.
	if (m_Components.find(type) != m_Components.end())
	{
	// If it does, return it.
		return m_Components[type];
	}
	// Otherwise, return null.
	return nullptr;
}

std::map<std::type_index, Component*>& Entity::getComponents()
{
	return m_Components;
}

/// @brief Flag an entity for destruction.
void Entity::Destroy() { m_IsDestroyed = true; }

/// @brief  Checks if an entity has been destroyed or not.
/// @return If the entity has been destroyed (bool).
bool Entity::Destroyed() { return m_IsDestroyed; }

/// @brief		Set the entity's name.
/// @param name Name of the entity.
void Entity::SetName(const std::string& name)
{
	// Make sure the name is valid.
	if (!name.empty())
	{
		m_Name = name;
	}
}

/// @brief  Get the name of the entity.
/// @return The name of the entity (string).
const std::string& Entity::GetName() { return m_Name; }

/// @brief		  Compares an entity's name.
/// @param name   The name to compare against.
/// @return		  Does the name match (bool)?
bool Entity::IsNamed(const std::string& name)
{
	if (!name.empty())
	{
		return (m_Name == name);
	}
	return false;
}

/// @brief		  Clone this entity from an archetype.
/// @param stream The json value to read from.
void Entity::ReadArchetype(Stream stream)
{
	/// TODO: Write this function.
}

/// @brief		  Read in the name of entity.
/// @param stream The json value to read from.
void Entity::ReadName(Stream stream)
{
	m_Name = stream.Read<std::string>();
}

/// @brief		  Read in the data for all the components of entity.
/// @param stream The json object to read from.
void Entity::ReadComponents(Stream stream)
{
	for ( auto& componentData : stream.GetObject() )
	{
		// Create the component found in the document.
		Component* component = ComponentFactory::Create(componentData.name.GetString());
		try
		{
			// Read in all the data for the component from the json.
			Stream(componentData.value).Read(component);
		}
		// Error handling 
		catch (std::runtime_error error)
		{
			std::cerr << error.what() << std::endl;
		}
		// Add the component to the entity.
		Add(component);
	}
}

/// @brief A map of the all read methods used by the Entity class.
ReadMethodMap< Entity > Entity::s_ReadMethods = {
	{ "Archetype"  , &ReadArchetype  },
	{ "Components" , &ReadComponents },
	{ "name"       , &ReadName	     }
};