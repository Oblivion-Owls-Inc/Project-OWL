///*****************************************************************/
/// @file	 WavesBehavior.h
/// @Author  Tyler Birdsall (tyler.birdsall@digipen.edu
/// @date	 10/16/2023
/// @brief   WavesBehavior class header
/// @details This class is used to create a base wave behavior 
///*****************************************************************/

#pragma once
#include "behavior.h"
#include "Transform.h"



///*****************************************************************/
/// WavesBehavior class
/// @brief This class contains the WavesBehavior component which is used
///			
///*****************************************************************/

class WavesBehavior :
    public Behavior
{
	public:

		WavesBehavior();
		WavesBehavior(const WavesBehavior& other);
		~WavesBehavior();
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
		int numWaves;
		int currentWave;
		int inspectorWave;
		int inspectorGroup;

		struct enemyGroup
		{
			enemyGroup();
			std::string name;
			int enemyAmount;
			float spawnInterval;
			float timer;
			float offset;
			int spawner;
		};

		struct Wave
		{
			Wave();
			std::vector<enemyGroup*> groups;
			float remainingTime;
			float timeToNextWave;
			int numGroups;
		};

		std::vector<Wave*> waves;

		std::vector<Transform*> spawners;

	public:

		float getTimer();

	private:

		void Spawn(std::string name, int group);

		void GuiWaves();

		void GuiGroups();

		void GuiCurrentWave();

		void GuiCurrentGroups();

		void GuiCurrent();

		void GuiSpawners();

	private: ///Reading 

		/// @brief reads the wave data from the json file
		/// @param jsonValue  the json data
		void readWaveData(nlohmann::ordered_json const& data);

		void readSpawners(nlohmann::ordered_json const& data);

		void readEData(nlohmann::ordered_json const& data);

		/// @brief the map of read methods for this Component
		static ReadMethodMap< WavesBehavior > const s_ReadMethods;

		/// @brief gets the map of read methods for this Component
		/// @return the map of read methods for this Component
        virtual ReadMethodMap< ISerializable > const& GetReadMethods() const override
        {
            return (ReadMethodMap< ISerializable > const&)s_ReadMethods;
        }

public:

	/// @brief Write all WavesBehavior data to a JSON file.
	/// @return The JSON file containing the WavesBehavior data.
	virtual nlohmann::ordered_json Write() const override;

};

