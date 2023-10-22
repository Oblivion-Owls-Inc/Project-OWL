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

using CollisionLayerFlags = unsigned;

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

		CollisionLayerFlags m_CollisionLayerFlags;
		float m_FireRate = 1.0f;
		float m_Range = 5.0f;
		float m_BulletDamage = 1.0f;
		float m_BulletSpeed = 1.0f;
		float m_BulletSize = 1.0f;
		float m_LastFireTime = 0.0f;
		std::string m_TargetName;
		std::string m_BulletName;
		Entity* m_BulletPrefab = nullptr; /// Todo: Make this a prefab actually work

	private:

		void FireBullet(RayCastHit target);
		RayCastHit CheckForTarget();
		void CheckIfBulletChanged();

	private: ///Reading 

		/// @brief reads in the name of the target
		/// @param data - the json data to read from
		void readTargetName(nlohmann::ordered_json const& data);

		/// @brief reads the fire rate from the json file
		/// @param jsonValue  the json data
		void readFireRate( nlohmann::ordered_json const& data );

		/// @brief reads the range from the json file
		/// @param jsonValue the json data
		void readRange( nlohmann::ordered_json const& data );

		/// @brief reads the bullet damage from the json file
		/// @param jsonValue the json data
		void readBulletDamage( nlohmann::ordered_json const& data );
		
		/// @brief reads the Bullet Speed from the json file
		/// @param jsonValue the json data
		void readBulletSpeed( nlohmann::ordered_json const& data );

		/// @brief reads the bullet size from the json file
		/// @param jsonValue the json data
		void readBulletSize( nlohmann::ordered_json const& data );

		/// @brief the map of read methods for this Component
		static ReadMethodMap< TurretBehavior > const s_ReadMethods;

		/// @brief gets the map of read methods for this Component
		/// @return the map of read methods for this Component
        virtual ReadMethodMap< ISerializable > const& GetReadMethods() const override
        {
            return (ReadMethodMap< ISerializable > const&)s_ReadMethods;
        }

public:

	/// @brief Write all TurretBehavior data to a JSON file.
	/// @return The JSON file containing the TurretBehavior data.
	virtual nlohmann::ordered_json Write() const override;

};

