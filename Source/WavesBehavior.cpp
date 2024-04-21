///--------------------------------------------------------------------------//
/// \file   WavesBehavior.cpp
/// \brief  Definitions for wave and spawning behavior
/// 
/// \author Tyler Birdsall (tyler.birdsall)
/// \date   10/16/23
///
/// \copyright Copyright (c) 2024 Digipen Institute of Technology
///--------------------------------------------------------------------------//
#include "pch.h" // precompiled header has to be included first
#include "WavesBehavior.h"
#include "BehaviorSystem.h"
#include "EntitySystem.h"
#include "Transform.h"
#include "Animation.h"
#include "DebugSystem.h"
#include "AssetLibrarySystem.h"
#include "Engine.h"
#include "Tilemap.h"
#include "RenderSystem.h"
#include "Texture.h"
#include "InputSystem.h"

//-----------------------------------------------------------------------------
// public: constructor / destructor
//-----------------------------------------------------------------------------

/// @brief default constructor
WavesBehavior::WavesBehavior() :
    Behavior(typeid(WavesBehavior)),
    numWaves(0),
    currentWave(0),
    inspectorWave(0),
    inspectorGroup(0)
{
    waves.clear();
    spawners.clear();
}

/// @brief cpy ctor
WavesBehavior::WavesBehavior(const WavesBehavior& other)
    : Behavior(typeid(WavesBehavior)),
    numWaves(other.numWaves),
    currentWave(other.currentWave),
    inspectorWave(other.inspectorWave),
    inspectorGroup(other.inspectorGroup)
{
    waitForEvent = other.waitForEvent;
    eventName = other.eventName;
    waves = other.waves;
    spawners = other.spawners;
}

/// @brief dtor
WavesBehavior::~WavesBehavior()
{
}

/// @brief wave default constructor
WavesBehavior::Wave::Wave() :
    timeToNextWave(0)
{}

/// @brief enemyGroup default constructor
WavesBehavior::EnemyGroup::EnemyGroup() :
    enemyAmount(0),
    spawnInterval(0),
    timer(0),
    offset(0),
    spawner(0)
{}

//-----------------------------------------------------------------------------
// private: copying
//-----------------------------------------------------------------------------

/// @brief	copy and return a new wave behavior
Component* WavesBehavior::Clone() const
{
    return (Component*)new WavesBehavior(*this);
}

//-----------------------------------------------------------------------------
// private: virtual override methods
//-----------------------------------------------------------------------------

/// @brief initialize WavesBehavior
void WavesBehavior::OnInit()
{
    /// Add this behavior to the behavior system
    Behaviors< WavesBehavior >()->AddComponent( this );

    for ( Wave& wave : waves )
    {
        for ( EnemyGroup& group : wave.groups )
        {
            group.enemy.SetOwnerName( "WavesBehavior" );
            group.enemy.Init();
        }
    }

    /// Set the filter function for the listener
    m_Listener.SetFilterFunction([&](std::string const& EventNameBegin) -> bool
    {
        return EventNameBegin == eventName;
    });

    /// Set the Callback function for the listener
    m_Listener.SetResponseFunction([&](std::string const& EventNameBegin)
    {
        waitForEvent = false;
    });

    m_Listener.Init();
}

/// @brief cleanup WavesBehavior
void WavesBehavior::OnExit()
{
    /// Remove this behavior from the behavior system
    Behaviors< WavesBehavior >()->RemoveComponent( this );

    m_Listener.Exit();
}

/// @brief update waves behavior and handle when things should spawn
void WavesBehavior::OnFixedUpdate()
{
    if (currentWave < numWaves && waitForEvent == false)
    {
        float dt = Engine::GetInstance()->GetFixedFrameDuration();
        if (waves[currentWave].timeToNextWave > 0.0f)
        {
            waves[currentWave].timeToNextWave -= dt;
            for (int i = 0; i < waves[currentWave].groups.size(); i++)
            {
                if (waves[currentWave].groups[i].enemyAmount > 0)
                {
                    waves[currentWave].groups[i].timer -= dt;
                    if (waves[currentWave].groups[i].timer < 0.0f)
                    {
                        waves[currentWave].groups[i].timer +=
                            waves[currentWave].groups[i].spawnInterval;
                        waves[currentWave].groups[i].enemyAmount--;
                        int size = (int)spawners.size();
                        if (waves[currentWave].groups[i].spawner >= size)
                        {
                            waves[currentWave].groups[i].spawner = size - 1;
                        }
                        if (waves[currentWave].groups[i].spawner < 0)
                        {
                            waves[currentWave].groups[i].spawner = 0;
                        }
                        Spawn(waves[currentWave].groups[i].enemy, i);
                    }
                }
            }
        }
        if (waves[currentWave].timeToNextWave <= 0.0f)
        {
            waves[currentWave].timeToNextWave = 0.0f;
            currentWave++;
        }
    }
}

//-----------------------------------------------------------------------------
// public: accessors
//-----------------------------------------------------------------------------

/// @brief get the timer for a UI element
/// @brief this should probably be fancier
float WavesBehavior::getTimer()
{
    return waves[currentWave].timeToNextWave;
}

//-----------------------------------------------------------------------------
// private: methods
//-----------------------------------------------------------------------------

/// @brief spawn an enemy with given name from given entity group
/// @param enemy - the enemy to spawn
/// @param group - enemyGroup to spawn from
void WavesBehavior::Spawn(Entity const* enemy, int group)
{
    if (enemy)
    {
        Entity* copy = enemy->Clone();
        Transform* copyTransform = copy->GetComponent<Transform>();
        if (spawners.size() == 0)
        {
            glm::vec2 vec(0, 0);
            copyTransform->SetTranslation(vec);
        }
        else
        {
            copyTransform->SetTranslation(spawners[waves[currentWave].groups[group].spawner]);
        }
        copy->AddToScene();
    }
}

//-----------------------------------------------------------------------------
// private: reading
//-----------------------------------------------------------------------------

/// @brief read if we wait for an event
/// @param data the data to read from.
void WavesBehavior::readWait(nlohmann::ordered_json const& data)
{
    waitForEvent = Stream::Read<bool>(data);
}

/// @brief read the waves data
/// @param data the data to read from.
void WavesBehavior::readEvent(nlohmann::ordered_json const& data)
{
    eventName = Stream::Read<std::string>(data);
}

/// @brief read the waves data
/// @param data the data to read from.
void WavesBehavior::readWaves(nlohmann::ordered_json const& data)
{
    numWaves = (int)data.size();
    waves.resize(data.size());
    for (int i = 0; i < data.size(); ++i)
    {
        Stream::Read(waves[i], data[i]);
    }
}

/// @brief read the alternate spawners data
/// @param data the data to read from.
void WavesBehavior::readSpawners(nlohmann::ordered_json const& data)
{
    for (nlohmann::ordered_json const& location : data)
    {
        spawners.push_back(Stream::Read<2, float>(location));
    }
}

/// @brief read the time to next wave
void WavesBehavior::Wave::readNextTime(nlohmann::ordered_json const& json)
{
    timeToNextWave = Stream::Read<float>(json);
}

/// @brief read groups in a wave
void WavesBehavior::Wave::readGroups(nlohmann::ordered_json const& json)
{
    groups.resize(json.size());
    for (int i = 0; i < json.size(); ++i)
    {
        Stream::Read(groups[i], json[i]);
    }
}

/// @brief read the enemy of an enemy group
void WavesBehavior::EnemyGroup::readEnemy(nlohmann::ordered_json const& json)
{
    Stream::Read( enemy, json );
}

/// @brief read the enemy amount of an enemy group
void WavesBehavior::EnemyGroup::readAmount(nlohmann::ordered_json const& json)
{
    enemyAmount = Stream::Read<int>(json);
}

/// @brief read the spawn interval of an enemy group
void WavesBehavior::EnemyGroup::readInterval(nlohmann::ordered_json const& json)
{
    spawnInterval = Stream::Read<float>(json);
}

/// @brief read the time offset of an enemy group
void WavesBehavior::EnemyGroup::readOffset(nlohmann::ordered_json const& json)
{
    offset = Stream::Read<float>(json);
    timer = offset;
}

/// @brief read the spawner number of an enemy group
void WavesBehavior::EnemyGroup::readSpawner(nlohmann::ordered_json const& json)
{
    spawner = Stream::Read<int>(json);
}

/// @brief Write all WavesBehavior data to a JSON file.
/// @return The JSON file containing the WavesBehavior data.
nlohmann::ordered_json WavesBehavior::Write() const
{
    nlohmann::ordered_json data;

    data["Wait"] = waitForEvent;
    data["Event"] = eventName;

    nlohmann::ordered_json& altSpawn = data["Spawners"];
    for (int i = 0; i < spawners.size(); i++)
    {
        altSpawn.push_back(Stream::Write(spawners[i]));
    }

    nlohmann::ordered_json& writeWaves = data["Waves"];
    for (int i = 0; i < numWaves; i++)
    {
        writeWaves.push_back(waves[i].Write());
    }

    return data;
}

/// @brief	write a wave to json
nlohmann::ordered_json WavesBehavior::Wave::Write() const
{
    nlohmann::ordered_json data;

    data["WaveNextTime"] = timeToNextWave;
    nlohmann::ordered_json& eGroups = data["EnemyGroups"];
    for (int i = 0; i < groups.size(); i++)
    {
        eGroups.push_back(groups[i].Write());
    }

    return data;
}

/// @brief	write an enemy group to json
nlohmann::ordered_json WavesBehavior::EnemyGroup::Write() const
{
    nlohmann::ordered_json data;

    data["Enemy"] = Stream::Write( enemy );
    data["Amount"] = enemyAmount;
    data["SpawnInterval"] = spawnInterval;
    data["InitialOffset"] = offset;
    data["Spawner"] = spawner;


    return data;
}

/// @brief read method map
ReadMethodMap<WavesBehavior> const WavesBehavior::s_ReadMethods =
{
    { "Wait",		           &readWait},
    { "Event",		          &readEvent},
    { "Spawners",		   &readSpawners},
    { "Waves",				  &readWaves},
};

/// @brief read method map for a wave
ReadMethodMap<WavesBehavior::Wave> const WavesBehavior::Wave::s_ReadMethods =
{
    { "WaveNextTime",		  &readNextTime},
    { "EnemyGroups",		  &readGroups},
};

/// @brief read method map for an enemy group
ReadMethodMap<WavesBehavior::EnemyGroup> const WavesBehavior::EnemyGroup::s_ReadMethods =
{
    { "Enemy",                &readEnemy},
    { "Amount",				  &readAmount},
    { "SpawnInterval",		  &readInterval},
    { "InitialOffset",		  &readOffset},
    { "Spawner",			  &readSpawner},
};