/**
* @file Component.h
* @author Tyler Birdsall (tyler.birdsall@digipen.edu)
* @brief Virtual component header
* @version 0.1
* @date 2023-09-05
* 
* @copyright Copyright (c) 2023
* 
*/

#pragma once

#include <typeindex>
#include <map>
#include <string>
#include "Stream.h"

class Entity;

template < typename ComponentType >
using ReadMethod = void (ComponentType::*)( Stream jsonValue );


/// @brief virtual component class
class Component
{
public:

	/// @brief gets the components type
	/// @return component type
	__inline std::type_index Type() const { return type; }

	/// @brief virtual destructor
	virtual ~Component() {};

	/// @brief sets the parent entity of the component
	/// @param parent, the parent entity of the component
	__inline void Parent( Entity* parent_ ) { parent = parent_; }

	/// @brief returns the components parent entity
	/// @return the parent entity of the component
	__inline Entity* Parent() const { return parent; }
	
	/// @brief virtual component clone function
	/// @return new clone of component
	virtual Component* Clone() const = 0;

	virtual ReadMethodMap< Component > const& GetReadMethods() const = 0;

protected:

	/// @brief default component constructor
	/// @param type what type of component this is
	Component( std::type_index type );

    /// @brief copy constructor
    /// @param the component to clone
    Component( Component const& other );

private:

	std::type_index type;

	Entity* parent;

};
