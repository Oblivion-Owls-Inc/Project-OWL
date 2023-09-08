#include "Entity.h"
#include <algorithm> // std::sort

//------------------------------------------------------------------------------
// Public Functions:
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
// Function:    Constrcutor
// Description: Dynamically allocate an entity.
// Input:		None
// Output:		An entity is created.
//------------------------------------------------------------------------------
Entity::Entity()
	: mName("")
	, mIsDestroyed(false)
	, components()
{
}

//------------------------------------------------------------------------------
// Function:    Copy Constrcutor
// Description: Creates an entity by copying another one
// Input:		other - reference to another entity
// Output:		An entity is created.
//------------------------------------------------------------------------------
Entity::Entity(const Entity& other)
	: mName(other.mName)
	, mIsDestroyed(other.mIsDestroyed)
{
	for (auto component : other.components)
	{
		Component* clone = component->Clone();
		Add(clone);
	}
}

//------------------------------------------------------------------------------
// Function:    Destructor
// Description: Destrys an entity
// Input:		None
// Output:		An entity is destroyed.
//------------------------------------------------------------------------------
Entity::~Entity() { Free(); }

//------------------------------------------------------------------------------
// Function:    Clone
// Description: Clones an entity
// Input:		other - reference to an entity
// Output:		A pointer to the new entity (Entity*)
//------------------------------------------------------------------------------
Entity* Entity::Clone() const { return new Entity(*this); }

//------------------------------------------------------------------------------
// Function:    Free
// Description: Deallocates all the memory used by an entity
// Input:		entity - pointer to an entity pointer
// Output:		All the memory deallocated (void).
//------------------------------------------------------------------------------
void Entity::Free()
{
	// Traverse the component list
	for (auto component : components)
	{
		// Make sure the component is valid.
		if (component)
		{
			// Delete the component.
			delete component;
		}
	}
	// Clear the component list.
	components.clear();
}

//------------------------------------------------------------------------------
// Function:    Add
// Description: Adds a component to an entity
// Input:		component - the component to be added
// Output:		The component's parent is set and the component
//				is added to entity's component list (void).
//------------------------------------------------------------------------------
void Entity::Add(Component* component)
{
	// Make sure the parameter is valid
	if (component)
	{
		// Set the component's parent as this entity
		component->Parent(this);
		// Add the component to the components list
		components.push_back(component);
		// Sort the list for faster access using a binary search
		//std::sort(components.begin(), components.end(), Component::Comparator);
	}
}

//------------------------------------------------------------------------------
// Function:    Destroy
// Description: Flag an entity for destruction
// Input:		None
// Output:		The entity is marked for destruction (void).
//------------------------------------------------------------------------------
void Entity::Destroy() { mIsDestroyed = true; }

//------------------------------------------------------------------------------
// Function:    Destroyed
// Description: Checks whether an entity has been flagged for destruction
// Input:		None
// Output:		Whether the entity has been marked for destruction (bool).
//------------------------------------------------------------------------------
bool Entity::Destroyed() { return mIsDestroyed; }

//------------------------------------------------------------------------------
// Function:    SetName
// Description: Set the entity's name
// Input:		name - the entity's new name
// Output:		The entity has a new name (void).
//------------------------------------------------------------------------------
void Entity::SetName(const std::string& name)
{
	// Make sure the name is valid.
	if (!name.empty())
	{
		mName = name;
	}
}

//------------------------------------------------------------------------------
// Function:    GetName
// Description: Get the entity's name
// Input:		None
// Output:		The name of the entity (string).
//------------------------------------------------------------------------------
const std::string& Entity::GetName() { return mName; }

//------------------------------------------------------------------------------
// Function:    IsNamed
// Description: Compare the entity's name with the specified name
// Input:		name - the specified name
// Output:		Whether the two names match
//------------------------------------------------------------------------------
bool Entity::IsNamed(const std::string& name)
{
	if (!name.empty())
	{
		return (mName == name);
	}
	return false;
}