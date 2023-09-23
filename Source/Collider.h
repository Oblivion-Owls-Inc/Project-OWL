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
#include "rapidjson.h"

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
			ColliderTypeNone = 0,
			ColliderTypeCircle,
			ColliderTypeLine,

		} ColliderType;

	public:
		Collider(const Collider& other);
		~Collider() = default;
		virtual Component* Clone() const override;
		virtual void OnFixedUpdate();
		// change name 
		virtual bool CheckIfColliding(const Collider* other) = 0;
		virtual ColliderType getColliderType() const { return mtype; }

	public:
		void setOtherCollider(Collider* other);
		Collider* getOtherCollider();
		bool isColliding();
		void isColliding(bool colliding);
		void setColliderType(ColliderType cType);

	private: // Read Methods
		virtual std::map< std::string, ReadMethod< Component > > const& getReadMethods();
		void ReadColliderType(Stream data);
		static ReadMethodMap< Collider > readMethods;
	private:
		ColliderType mtype;
		bool mIsColliding;
		Collider* mOther; // Instagator is the other collider that is colliding with this collider
};
