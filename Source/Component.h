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
class Entity;


class Component
{
public:

	__inline std::type_index Type() const { return type; }

	virtual ~Component() {};

	__inline void Parent( Entity* parent_ ) { parent = parent_; }

	__inline Entity* Parent() const { return parent; }
	
	virtual Component* Clone() const = 0;

protected:

	Component( std::type_index type );

private:

	std::type_index type;

	Entity* parent = nullptr;

};
