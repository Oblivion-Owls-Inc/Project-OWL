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
class Transform;
class AudioPlayer;

class TurretBehavior : public Behavior
{
//-------------------------------------------------------------------------------------------
public: // constructor
//-------------------------------------------------------------------------------------------

	/// @brief  constructor
	TurretBehavior();

//-------------------------------------------------------------------------------------------
private: // virtual override methods
//-------------------------------------------------------------------------------------------

    /// @brief  called once when entering the scene
    virtual void OnInit();

    /// @brief  called when this Component's Entity is removed from the Scene
    /// @note   NOT CALLED WHEN THE SCENE IS EXITED - that should be handled by this Component's System
    virtual void OnExit();

    /// @brief Called Every Fixed Frame by the system
    void OnFixedUpdate() override;

    /// @brief Used by the Debug System to display information about this Component
    virtual void Inspector() override;

///-------------------------------------------------------------------------------------------
private: /// Members
///-------------------------------------------------------------------------------------------

	bool m_IsActive = false;

	/// @brief The fire rate of the turret
	float m_FireRate = 1.0f;

	/// @brief The range of the turret
	float m_Range = 5.0f;

	/// @brief The damage of the turret
	int m_BulletDamage = 1;

	/// @brief The speed of the bullet
	float m_BulletSpeed = 1.0f;

	/// @brief The size of the bullet
	float m_BulletSize = 1.0f;

	/// @brief  the time until the turret can fire again
	float m_FireCooldown = 0.0f;

	/// @brief Name of The entity the turret is targeting
	std::string m_TargetName;

	/// @brief The bullet prefab to spawn
	Entity const* m_BulletPrefab = nullptr;

    /// @brief  the Transform of this Turret
    Transform* m_Transform = nullptr;

	/// @brief the AudioPlayer of this Turret
	AudioPlayer* m_AudioPlayer = nullptr;

///-------------------------------------------------------------------------------------------
private: // methods
///-------------------------------------------------------------------------------------------

	/// @brief  creates a bullet and fires it at the target
    /// @param  direction the direction to fire the bullet in
	void FireBullet( glm::vec2 const& direction );

	void checkActive();

	/// @brief  Uses Raycasting to check for a target on the same Collision Layer
	/// @return the direction towards the target, or (0, 0) if no valid target found
	glm::vec2 CheckForTarget();

///-------------------------------------------------------------------------------------------
private: // reading 
///-------------------------------------------------------------------------------------------

	/// @brief Reads the name of the bullet prefab to grab from AssetLib
	/// @param data - the json data to read from
	void readBulletName(nlohmann::ordered_json const& data);

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


///-------------------------------------------------------------------------------------------
public: // reading / writing
///-------------------------------------------------------------------------------------------

    /// @brief gets the map of read methods for this Component
    /// @return the map of read methods for this Component
    virtual ReadMethodMap< ISerializable > const& GetReadMethods() const override
    {
        return (ReadMethodMap< ISerializable > const&)s_ReadMethods;
    }

	/// @brief Write all TurretBehavior data to a JSON file.
	/// @return The JSON file containing the TurretBehavior data.
	virtual nlohmann::ordered_json Write() const override;

///-------------------------------------------------------------------------------------------
public: // copying
///-------------------------------------------------------------------------------------------

    /// @brief  creates a new clone of this Component
    /// @return the newly created clone
    virtual TurretBehavior* Clone() const override
    {
        return new TurretBehavior( *this );
    }

	void turretSetActive(bool state);

///-------------------------------------------------------------------------------------------
private: // copying
///-------------------------------------------------------------------------------------------
    
    /// @brief  copy constructor
    /// @param  other   the TurretBehavior to copy
    TurretBehavior(TurretBehavior const& other);

///-------------------------------------------------------------------------------------------
};
