///*****************************************************************/
/// \file	 WavesBehavior.h
/// \author  Tyler Birdsall (tyler.birdsall@digipen.edu
/// \date	 10/16/2023
/// \brief   WavesBehavior class header
/// \details This class is used to create a base wave behavior 
/// 
/// \copyright Copyright (c) 2024 Digipen Institute of Technology
///*****************************************************************/

#pragma once



#include "EventSystem.h"


#include "EventListener.h"



///*****************************************************************/
/// WavesBehavior class
/// @brief This class contains the WavesBehavior component which is used
///			
///*****************************************************************/

class WavesBehavior :
	public Behavior
{

//-----------------------------------------------------------------------------
public: // constructor / destructor
//-----------------------------------------------------------------------------

	/// @brief default constructor
	WavesBehavior();
	/// @brief cpy ctor
	WavesBehavior(const WavesBehavior& other);
	/// @brief dtor
	~WavesBehavior();
	/// @brief wave default constructor
	Component* Clone() const override;

//-----------------------------------------------------------------------------
private: // virtual override methods
//-----------------------------------------------------------------------------


	/// @brief Default constructor for the RigidBody class.
	virtual void OnInit();

	/// @brief  called when this Component's Entity is removed from the Scene
	/// @note   NOT CALLED WHEN THE SCENE IS EXITED - that should be handled by this Component's System
	virtual void OnExit();


	/// @brief Called Every Fixed Frame by the system
	virtual void OnFixedUpdate() override;


//-----------------------------------------------------------------------------
public: // inspection
//-----------------------------------------------------------------------------


	/// @brief Used by the Debug System to display information about this Component
	virtual void Inspector() override;

    
//-----------------------------------------------------------------------------
private: // inspector methods
//-----------------------------------------------------------------------------


	/// @brief displays wave data to edit
	void guiWaves();

	/// @brief displays group data to edit
	void guiGroups();

	/// @brief displays the currently in view wave data
	void guiInViewWave();

	/// @brief displays the currently in view enemy groups data
	void guiInViewGroups();

	/// @brief calls the in view gui handlers
	void guiInView();

	/// @brief displays the currently active wave data
	void guiCurrentWave();

	/// @brief displays the currently active enemy groups data
	void guiCurrentGroups();

	/// @brief calls the current gui handlers
	void guiCurrent();

	/// @brief lists all spawners and locations
	void guiSpawners();

	/// @brief lists all spawners and locations
	void guiEvent();

	/// @brief	allows dragging the spawner to change location
	void debugDrag(int number);

	/// @brief	adds a wave to the behavior
	void guiAddWave();

	/// @brief	removes a wave from the behavior
	void guiRemoveWave();

	/// @brief	adds a group to the behavior
	void guiAddGroup();

	/// @brief	removes a group from the behavior
	void guiRemoveGroup();

	/// @brief	adds a spawner to the behavior
	void guiAddSpawner();

	/// @brief	removes a spawner from the behavior
	void guiRemoveSpawner();


//-----------------------------------------------------------------------------
private: // data
//-----------------------------------------------------------------------------


	int numWaves;
	int currentWave;
	int inspectorWave;
	int inspectorGroup;

	class EnemyGroup : public ISerializable
	{
	public:

		/// @brief enemyGroup default constructor
		EnemyGroup();
		AssetReference< Entity > enemy;
		int enemyAmount;
		float spawnInterval;
		float timer;
		float offset;
		int spawner;

	private:

		/// @brief read the name of an enemy group
		void readEnemy(nlohmann::ordered_json const& json);

		/// @brief read the enemy amount of an enemy group
		void readAmount(nlohmann::ordered_json const& json);

		/// @brief read the spawn interval of an enemy group
		void readInterval(nlohmann::ordered_json const& json);

		/// @brief read the time offset of an enemy group
		void readOffset(nlohmann::ordered_json const& json);

		/// @brief read the spawner number of an enemy group
		void readSpawner(nlohmann::ordered_json const& json);

		/// @brief the map of read methods for this Component
		static ReadMethodMap< EnemyGroup > const s_ReadMethods;

	public:

		/// @brief read method map for an enemy group
		virtual ReadMethodMap< ISerializable > const& GetReadMethods() const override
		{
			return (ReadMethodMap< ISerializable > const&)s_ReadMethods;
		}

		/// @brief	write an enemy group to json
		virtual nlohmann::ordered_json Write() const override;

	};

	// wave class
	class Wave : public ISerializable
	{
	public:
		/// @brief wave default constructor
		Wave();
		// container of attached groups
		std::vector<EnemyGroup> groups;
		float timeToNextWave;

	private:

		/// @brief read the time in a wave	
		void readWaveTime(nlohmann::ordered_json const& json);

		/// @brief read the time to next wave
		void readNextTime(nlohmann::ordered_json const& json);

		/// @brief read groups in a wave
		void readGroups(nlohmann::ordered_json const& json);

		/// @brief the map of read methods for this Component
		static ReadMethodMap< Wave > const s_ReadMethods;

	public:

		/// @brief read method map for a wave
		virtual ReadMethodMap< ISerializable > const& GetReadMethods() const override
		{
			return (ReadMethodMap< ISerializable > const&)s_ReadMethods;
		}

		/// @brief	write a wave to json
		virtual nlohmann::ordered_json Write() const override;
	};

	// container for the waves
	std::vector<Wave> waves;

	// container for the spawner locations
	std::vector< glm::vec2 > spawners;

	// should the wave wait for an event
	bool waitForEvent = false;

	// name of event to wait for
	std::string eventName;

	/// @brief Listener for the event
	EventListener<std::string> m_Listener;


//-----------------------------------------------------------------------------
public: // accessors
//-----------------------------------------------------------------------------

	/// @brief get the timer for a UI element
	/// @brief this should probably be fancier
	float getTimer();

//-----------------------------------------------------------------------------
private: // methods
//-----------------------------------------------------------------------------

	/// @brief spawn an enemy with given name from given entity group
	/// @param enemy - the enemy to spawn
	/// @param group - enemyGroup to spawn from
	void Spawn(Entity const* enemy, int group);

//-----------------------------------------------------------------------------
private: // reading
//-----------------------------------------------------------------------------

	/// @brief read if we wait for an event
	/// @param data the data to read from.
	void readWait(nlohmann::ordered_json const& data);

	/// @brief read the event
	/// @param data the data to read from.
	void readEvent(nlohmann::ordered_json const& data);

	/// @brief read the waves data
	/// @param data the data to read from.
	void readWaves(nlohmann::ordered_json const& data);

	/// @brief read the alternate spawners data
	/// @param data the data to read from.
	void readSpawners(nlohmann::ordered_json const& data);

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

