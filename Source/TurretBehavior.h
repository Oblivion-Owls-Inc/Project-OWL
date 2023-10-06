///*****************************************************************/
/// @file	 TurretBehavior.h
/// @Author  Jax Clayton (jax.clayton@digipen.edu)
/// @date	 10/1/2023
/// @brief   TurretBehavior class header
/// @details This class is used to create a base turret behavior 
///*****************************************************************/

#pragma once
#include "behavior.h"



///*****************************************************************/
/// TurretBehavior class
/// @brief This class contains the TurretBehavior component which is used
///			
///*****************************************************************/

class TurretBehavior :
    public Behavior
{
	public:

		TurretBehavior();
		TurretBehavior(const TurretBehavior& other);
		~TurretBehavior();
		Component* Clone() const override;

		/// @brief  Called whenever a Collider on this Behavior's Entity collides
		/// @param  other           the entity that was collided with
		/// @param  collisionData   additional data about the collision
		virtual void OnCollision( Entity* other, CollisionData const& collisionData ) {};


		/// @brief Default constructor for the RigidBody class.
		virtual void OnInit();

		/// @brief  called when this Component's Entity is removed from the Scene
		/// @note   NOT CALLED WHEN THE SCENE IS EXITED - that should be handled by this Component's System
		virtual void OnExit();


	public:
		/// @brief Called Every Frame by the system
		/// @param dt - the time since the last frame
		virtual void OnUpdate(float dt) override;

		/// @brief Called Every Fixed Frame by the system
		void OnFixedUpdate() override;

		/// @brief Used by the Debug System to display information about this Component
		virtual void Inspector() override;

	private:
		float m_FireRate = 1.0f;
		float m_Range = 5.0f;
		float m_BulletDamage = 1.0f;
		float m_BulletSpeed = 1.0f;
		float m_BulletSize = 1.0f;
		Entity* m_BulletPrefab = nullptr; /// Todo: Make this a prefab actually work

	private:

		void FireBullet();

	private: ///Reading 

		/// @brief reads the fire rate from the json file
		/// @param jsonValue  the json data
		void ReadFireRate(Stream jsonValue);

		/// @brief reads the range from the json file
		/// @param jsonValue the json data
		void ReadRange(Stream jsonValue);

		/// @brief reads the bullet damage from the json file
		/// @param jsonValue the json data
		void ReadBulletDamage(Stream jsonValue);
		
		/// @brief reads the Bullet Speed from the json file
		/// @param jsonValue the json data
		void ReadBulletSpeed(Stream jsonValue);

		/// @brief reads the bullet size from the json file
		/// @param jsonValue the json data
		void ReadBulletSize(Stream jsonValue);

		/// @brief the map of read methods for this Component
		static ReadMethodMap< TurretBehavior > const readMethods;

		/// @brief gets the map of read methods for this Component
		/// @return the map of read methods for this Component
		virtual ReadMethodMap< Component > const& GetReadMethods() const override;

};

