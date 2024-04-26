///*****************************************************************/
/// \file	 TurretBehavior.h
/// \author  Jax Clayton (jax.clayton@digipen.edu)
/// \date	 10/1/2023
/// \brief   TurretBehavior class header
/// \details This class is used to create a base turret behavior
/// 
/// \copyright Copyright (c) 2024 Digipen Institute of Technology 
///*****************************************************************/

#pragma once





#include "AudioPlayer.h"
#include "Animation.h"




#include "CollisionLayerFlags.h"


///*****************************************************************/
/// TurretBehavior class
/// @brief This class contains the TurretBehavior component which is used
///			
///*****************************************************************/

class TurretBehavior : public Behavior
{
//-----------------------------------------------------------------------------
public: // constructor
//-----------------------------------------------------------------------------


	/// @brief  constructor
	TurretBehavior();


//-----------------------------------------------------------------------------
public: // virtual override methods
//-----------------------------------------------------------------------------


    /// @brief  called once when entering the scene
    virtual void OnInit();

    /// @brief  called when this Component's Entity is removed from the Scene
    virtual void OnExit();


    /// @brief Called Every Fixed Frame by the system
    void OnFixedUpdate() override;


//-----------------------------------------------------------------------------
private: /// Members
//-----------------------------------------------------------------------------


	/// @brief is the turret active
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


    /// @brief  layers that can obstruct line of sight between this Turret and its target
    CollisionLayerFlags m_ObstructionLayers = 0;


	/// @brief The bullet prefab to spawn
	AssetReference< Entity > m_BulletPrefab;

	/// @brief The animation of the turret
	ComponentReference< Animation > m_Animation;

    /// @brief  the Transform of this Turret
    ComponentReference< Transform > m_Transform;

	/// @brief the AudioPlayer of this Turret
	ComponentReference< AudioPlayer > m_AudioPlayer;

	/// @brief the Animation of this Turret

//-----------------------------------------------------------------------------
private: // methods
//-----------------------------------------------------------------------------


	/// @brief  creates a bullet and fires it at the target
    /// @param  direction the direction to fire the bullet in
	void fireBullet( glm::vec2 const& direction );


	/// @brief  checks if this Turret is in range of a Generator and updates it's active state
	void checkActive();


	/// @brief  Uses Raycasting to check for a target on the same Collision Layer
	/// @return the direction towards the target, or (0, 0) if no valid target found
	glm::vec2 checkForTarget();

    
//-----------------------------------------------------------------------------
public: // inspection
//-----------------------------------------------------------------------------


    /// @brief Used by the Debug System to display information about this Component
    virtual void Inspector() override;

//-----------------------------------------------------------------------------
public: // accessors
//-----------------------------------------------------------------------------

	/// @brief	get the turrets range
	/// @return	range of the turret
	float GetRange() const { return m_Range; }

//-----------------------------------------------------------------------------
private: // reading 
//-----------------------------------------------------------------------------


	/// @brief  reads the fire rate from the json file
	/// @param  data - the json data to read from
	void readFireRate( nlohmann::ordered_json const& data );

	/// @brief  reads the range from the json file
	/// @param  data - the json data to read from
	void readRange( nlohmann::ordered_json const& data );

	/// @brief  reads the bullet damage from the json file
	/// @param  data - the json data to read from
	void readBulletDamage( nlohmann::ordered_json const& data );
		
	/// @brief  reads the Bullet Speed from the json file
	/// @param  data - the json data to read from
	void readBulletSpeed( nlohmann::ordered_json const& data );

	/// @brief  reads the bullet size from the json file
	/// @param  data - the json data to read from
	void readBulletSize( nlohmann::ordered_json const& data );

    /// @brief  reads layers that can obstruct line of sight between this Turret and its target
    /// @param  data - the json data to read from
    void readObstructionLayers( nlohmann::ordered_json const& data );

    /// @brief  Reads the name of the bullet prefab to grab from AssetLib
    /// @param  data - the json data to read from
    void readBulletPrefab(nlohmann::ordered_json const& data);


//-----------------------------------------------------------------------------
public: // reading / writing
//-----------------------------------------------------------------------------


    /// @brief gets the map of read methods for this Component
    /// @return the map of read methods for this Component
    virtual ReadMethodMap< ISerializable > const& GetReadMethods() const override;


	/// @brief Write all TurretBehavior data to a JSON file.
	/// @return The JSON file containing the TurretBehavior data.
	virtual nlohmann::ordered_json Write() const override;


//-----------------------------------------------------------------------------
public: // copying
//-----------------------------------------------------------------------------


    /// @brief  creates a new clone of this Component
    /// @return the newly created clone
    virtual TurretBehavior* Clone() const override;


//-----------------------------------------------------------------------------
private: // copying
//-----------------------------------------------------------------------------
    

    /// @brief  copy constructor
    /// @param  other   the TurretBehavior to copy
    TurretBehavior( TurretBehavior const& other );


    // diable assignment operator
    void operator =( TurretBehavior const& ) = delete;


//-----------------------------------------------------------------------------
};
