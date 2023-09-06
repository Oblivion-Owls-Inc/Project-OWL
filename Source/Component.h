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

	__inline void Parent(Entity* parent) { cParent = parent; }

	__inline Entity* Parent() const { return cParent; }
	
	virtual Component* Clone() const = 0;

protected:
	std::type_index type;
private:
	Entity* cParent = nullptr;
};
