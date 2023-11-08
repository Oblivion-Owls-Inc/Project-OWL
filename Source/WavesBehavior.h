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

	/// @brief  Called whenever a Collider on this Behavior's Entity collides
	/// @param  other           the entity that was collided with
	/// @param  collisionData   additional data about the collision
	virtual void OnCollision(Entity* other, CollisionData const& collisionData) {};


	/// @brief Default constructor for the RigidBody class.
	virtual void OnInit();

	/// @brief  called when this Component's Entity is removed from the Scene
	/// @note   NOT CALLED WHEN THE SCENE IS EXITED - that should be handled by this Component's System
	virtual void OnExit();

	/// @brief Called Every Frame by the system
	/// @param dt - the time since the last frame
	virtual void OnUpdate(float dt) override;

	/// @brief Called Every Fixed Frame by the system
	void OnFixedUpdate() override;

//-----------------------------------------------------------------------------
private: // inspector methods
//-----------------------------------------------------------------------------

	/// @brief Used by the Debug System to display information about this Component
	virtual void Inspector() override;

	/// @brief displays wave data to edit
	void GuiWaves();

	/// @brief displays group data to edit
	void GuiGroups();

	/// @brief displays the currently active wave data
	void GuiCurrentWave();

	/// @brief displays the currently active enemy groups data
	void GuiCurrentGroups();

	/// @brief calls the current gui handlers
	void GuiCurrent();

	/// @brief lists all spawners and locations
	void GuiSpawners();


	void DebugDrag(int number);

	void GuiAddWave();
	void GuiRemoveWave();
	void GuiAddGroup();
	void GuiRemoveGroup();
	void GuiAddSpawner();
	void GuiRemoveSpawner();

//-----------------------------------------------------------------------------
private: // data
//-----------------------------------------------------------------------------

	int numWaves;
	int currentWave;
	int inspectorWave;
	int inspectorGroup;
	const Entity* base;

	class EnemyGroup : public ISerializable
	{
	public:

		/// @brief enemyGroup default constructor
		EnemyGroup();
		std::string name;
		const Entity* enemy = nullptr;
		int enemyAmount;
		float spawnInterval;
		float timer;
		float offset;
		int spawner;

	private:

		void readName(nlohmann::ordered_json const& json);


		void readAmount(nlohmann::ordered_json const& json);


		void readInterval(nlohmann::ordered_json const& json);


		void readOffset(nlohmann::ordered_json const& json);


		void readSpawner(nlohmann::ordered_json const& json);

		/// @brief the map of read methods for this Component
		static ReadMethodMap< EnemyGroup > const s_ReadMethods;

	public:

		virtual ReadMethodMap< ISerializable > const& GetReadMethods() const override
		{
			return (ReadMethodMap< ISerializable > const&)s_ReadMethods;
		}

		virtual nlohmann::ordered_json Write() const override;

	};

	class Wave : public ISerializable
	{
	public:
		/// @brief wave default constructor
		Wave();
		std::vector<EnemyGroup> groups;
		float remainingTime;
		float timeToNextWave;

	private:

		void readWaveTime(nlohmann::ordered_json const& json);


		void readNextTime(nlohmann::ordered_json const& json);


		void readGroups(nlohmann::ordered_json const& json);

		/// @brief the map of read methods for this Component
		static ReadMethodMap< Wave > const s_ReadMethods;

	public:

		virtual ReadMethodMap< ISerializable > const& GetReadMethods() const override
		{
			return (ReadMethodMap< ISerializable > const&)s_ReadMethods;
		}

		virtual nlohmann::ordered_json Write() const override;
	};

	std::vector<Wave> waves;

	std::vector< glm::vec2 > spawners;

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
	/// @param name  - the name of the enemy to spawn
	/// @param group - enemyGroup to spawn from
	void Spawn(std::string name, int group);

//-----------------------------------------------------------------------------
private: // reading
//-----------------------------------------------------------------------------

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

