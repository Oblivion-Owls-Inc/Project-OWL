/**
* @file Behavior.h
* @author Tyler Birdsall (tyler.birdsall@digipen.edu)
* @brief Virtual behavior component header
* @version 0.1
* @date 2023-09-12
*
* @copyright Copyright (c) 2023
*
*/

#pragma once
#include "Component.h"

class Behavior : public Component
{

	protected:

		Behavior(std::type_index type) :
			Component(type)
		{}

	public:
		virtual Component* Clone() const = 0;

		virtual void OnCollision(Entity* other) = 0;

		virtual void OnUpdate(float dt) {};

		virtual void OnFixedUpdate() {};
};