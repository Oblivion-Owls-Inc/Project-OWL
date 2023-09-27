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
	// Copy Constructor
	Entity(const Entity& other);
	// Destructor
	~Entity();

	// Dynamically allocate a clone of an existing Entity.
	Entity* Clone() const;
	// Deallocate the memory associated with an Entity.
	void Free();
	// Attach a component to an Entity.
	void Add(Component* component);

	Component* HasComponent(std::type_index type);

	// Type safe method for accessing the components.
    template < typename ComponentType >
    ComponentType* GetComponent()
    {
        auto componentIterator = m_Components.find(typeid(ComponentType));
        if (componentIterator == m_Components.end())
        {
            return nullptr;
        }
        return static_cast<ComponentType*>(componentIterator->second);
    }
    
	std::map<std::type_index, Component*>& getComponents();
	template < typename ComponentType >
	std::vector<ComponentType*> GetAll(ComponentType type);
	
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

	static __inline ReadMethodMap< Entity > const& GetReadMethods() { return s_ReadMethods; }

private:
	// The name of the entity.
	std::string m_Name;
	// Flag to indicate the entity is dead and should be destroyed
	bool m_IsDestroyed;
	// Container for attached components
	std::map<std::type_index, Component*> m_Components;

	void ReadName(Stream stream);
	void ReadComponents(Stream stream);
	void ReadArchetype(Stream stream);


	static ReadMethodMap<Entity> s_ReadMethods;

};
//------------------------------------------------------------------------------

template<typename ComponentType>
inline std::vector<ComponentType*> Entity::GetAll(ComponentType type)
{
	std::vector<ComponentType*> componentsOfType;

	for each (auto component in m_Components) 
	{
		if (component->Type() == type)
		{
			componentsOfType.push_back(component);
		}
	}

}
