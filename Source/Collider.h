///*************************************************************************/
///@file  Collider.cpp
/// @Author Jax Clayton (jax.clayton@digipen.edu)
/// @date   9/5/2021
/// @brief  This is the Collider class implementation file
///*************************************************************************/

///*************************************************************************/
/// Includes
///*************************************************************************/
#pragma once
#include "Component.h"

///*************************************************************************/
/// @Class Collider
/// @brief This class contains the Collider component which is used for handling
///			collisions
///************************************************************************/

class Collider : public Component
{
	protected:
		Collider();

		typedef enum ColliderType
		{
			ColliderTypeNone,
			ColliderTypeCircle,
			ColliderTypeLine,

		} ColliderType;

	public:
		Collider(const Collider& other);
		~Collider() = default;
		virtual Component* Clone() const override;
		virtual void OnFixedUpdate();
		bool CheckCollision(const Collider* other);
		// change name 
		virtual bool IsColliding(const Collider* other) = 0;
		bool isColliding();
		void isColliding(bool colliding);
		void setColliderType(ColliderType cType);
	private:
		ColliderType mtype;
		bool mIsColliding;

};
