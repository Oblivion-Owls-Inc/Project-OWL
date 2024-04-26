/**
* \file Behavior.h
* \author Tyler Birdsall (tyler.birdsall@digipen.edu)
* \brief Virtual behavior component header
* \version 0.1
* \date 2023-09-12
*
* \copyright Copyright (c) 2024 Digipen Institute of Technology
*
*/

#pragma once

#include "CollisionData.h"

class Behavior : public Component
{

	protected:

		Behavior( std::type_index m_Type ) :
			Component( m_Type )
		{}

	public:
		virtual ~Behavior() = default;

		virtual void OnUpdate(float dt) {};

        virtual void OnFixedUpdate() {};  

};