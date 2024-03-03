///*****************************************************************/
/// @file	    Generator.cpp
/// @author     Tyler Birdsall (tyler.birdsall@digipen.edu)
/// @date	    01/12/2024
/// @brief      Generator class implimentation
/// @copyright  Digipen LLC (c) 2024
///*****************************************************************/

#include "Generator.h"

#include "ComponentSystem.h"
#include "BehaviorSystem.h"

#include "CollisionSystem.h"
#include "EnemyBehavior.h"

#include "Health.h"
#include "Emitter.h"
#include "EmitterSprite.h"

//-----------------------------------------------------------------------------
// constructor / destructor 
//-----------------------------------------------------------------------------

/// @brief  constructor
Generator::Generator() :
    Behavior( typeid( Generator ) )
{}

/// @brief  clone
Generator* Generator::Clone() const
{
    return new Generator(*this);
}

//-----------------------------------------------------------------------------
// virtual override methods
//-----------------------------------------------------------------------------

/// @brief	initialize Generator
void Generator::OnInit()
{
    Behaviors< Generator >()->AddComponent(this);
    Behaviors< Behavior >()->AddComponent(this);

    m_Collider.SetOnConnectCallback(
        [ this ]()
        {
            m_Collider->AddOnCollisionEnterCallback(
                GetId(),
                std::bind(&Generator::onCollisionEnter, this, std::placeholders::_1)
            );
        }
    );
    m_Collider.SetOnDisconnectCallback(
        [ this ]()
        {
            m_Collider->RemoveOnCollisionEnterCallback( GetId() );
        }
    );

    m_Collider   .Init( GetEntity() );
    m_AudioPlayer.Init( GetEntity() );
    m_Transform  .Init( GetEntity() );
    m_Health     .Init( GetEntity() );

    // yes I know this is wrong, but the component reference threw a fit 
    // - please show me how to make it a reference later
    m_Emitter = GetEntity()->GetComponent<Emitter>();
    if (m_IsActive)
    {
        m_GrowthRadius = m_PowerRadius;
        ParticleSystem::EmitData data = m_Emitter->GetEmitData();
        data.startAhead = m_GrowthRadius;
        m_Emitter->SetEmitData(data);
        m_Emitter->SetContinuous(true);
    }
    else
    {
        m_GrowthRadius = 0.0f;
        m_Emitter->SetContinuous(false);
    }
}

/// @brief	called on exit, handles loss state
void Generator::OnExit()
{
    Behaviors< Generator >()->RemoveComponent(this);
    Behaviors< Behavior >()->RemoveComponent(this);

    m_Collider   .Exit();
    m_AudioPlayer.Exit();
    m_Transform  .Exit();
    m_Health     .Exit();
}

void Generator::OnUpdate(float dt)
{
    if (m_ActivateRing)
    {
        m_GrowthRadius += m_RadiusSpeed * dt;
        if (m_GrowthRadius >= m_PowerRadius)
        {
            m_GrowthRadius = m_PowerRadius;
            m_ActivateRing = false;
        }
        ParticleSystem::EmitData data = m_Emitter->GetEmitData();
        data.startAhead = m_GrowthRadius;
        m_Emitter->SetEmitData(data);
        m_Emitter->SetContinuous(true);
    }
    else if (m_DeactivateRing)
    {
        m_GrowthRadius -= m_RadiusSpeed * dt;
        if (m_GrowthRadius <= 0.0f)
        {
            m_GrowthRadius = 0.0f;
            m_Emitter->SetContinuous(false);
            m_DeactivateRing = false;
        }
        ParticleSystem::EmitData data = m_Emitter->GetEmitData();
        data.startAhead = m_GrowthRadius;
        m_Emitter->SetEmitData(data);

    }
}

//-----------------------------------------------------------------------------
// copying
//-----------------------------------------------------------------------------

/// @brief  copy ctor
Generator::Generator(const Generator& other) :
    Behavior( other ),
    m_IsActive        ( other.m_IsActive         ),
    m_PowerRadius     ( other.m_PowerRadius      ),
    m_ActivationRadius( other.m_ActivationRadius ),
    m_Depth           ( other.m_Depth            )
{}

//-----------------------------------------------------------------------------
// accessors
//-----------------------------------------------------------------------------

/// @brief	returns the lowest generator in the level
/// @return the lowest generator
Generator* Generator::GetLowestGenerator()
{
    Generator* lowest = Components<Generator>()->GetComponents().front();
    for (auto& generator : Components<Generator>()->GetComponents())
    {
        if (generator->m_Depth > lowest->m_Depth)
        {
            lowest = generator;
        }
    }
    return lowest;
}

/// @brief activate the generator
void Generator::Activate() 
{ 
    m_IsActive = true;
    m_ActivateRing = true;
}

//-----------------------------------------------------------------------------
// private functions
//-----------------------------------------------------------------------------

/// @brief collision callback for generators
void Generator::onCollisionEnter(Collider* other)
{
    if (m_IsActive)
    {
        EnemyBehavior* enemy = other->GetEntity()->GetComponent<EnemyBehavior>();
        if (!enemy)
        {
            return;
        }

        m_Health->TakeDamage( enemy->GetDamage() );

        if (m_AudioPlayer)
        {
            m_AudioPlayer->Play();
        }
        enemy->GetEntity()->Destroy();

        if (m_Health->GetHealth()->GetCurrent() <= 0)
        {
            m_IsActive = false;
            m_DeactivateRing = true;
            m_Health->GetHealth()->Reset();
        }
    }
}

//-----------------------------------------------------------------------------
// inspector methods
//-----------------------------------------------------------------------------

/// @brief	inspector for generators
void Generator::Inspector()
{
    ImGui::InputFloat("Radius", &m_PowerRadius, 0.5f, 1.0f);
    ImGui::InputFloat("Activate Radius", &m_ActivationRadius, 0.5f, 1.0f);
    ImGui::InputFloat("Growth Speed", &m_RadiusSpeed);
    ImGui::InputInt("Depth", &m_Depth, 1, 5);
    ImGui::Checkbox("Active", &m_IsActive);
}

//-----------------------------------------------------------------------------
// reading
//-----------------------------------------------------------------------------

/// @brief read method map
ReadMethodMap<Generator> const Generator::s_ReadMethods =
{
    { "Radius"        , &readRadius  },
    { "ActivateRadius", &readARadius },
    { "Depth"         , &readDepth   },
    { "Active"        , &readActive  },
    { "GrowthSpeed"   , &readSpeed   },
};

/// @brief	read the raidus from json
void Generator::readRadius(nlohmann::ordered_json const& json)
{
    m_PowerRadius = Stream::Read<float>(json);
}

/// @brief	read the activation radius from json
void Generator::readARadius(nlohmann::ordered_json const& json)
{
    m_ActivationRadius = Stream::Read<float>(json);
}

/// @brief	read the depth from json
void Generator::readDepth(nlohmann::ordered_json const& json)
{
    m_Depth = Stream::Read<int>(json);
}

/// @brief	read if the generator starts active from json
void Generator::readActive(nlohmann::ordered_json const& json)
{
    m_IsActive = Stream::Read<bool>(json);
}

/// @brief	read the speed the particle ring changes
void Generator::readSpeed(nlohmann::ordered_json const& json)
{
    m_RadiusSpeed = Stream::Read<float>(json);
}

//-----------------------------------------------------------------------------
// writing
//-----------------------------------------------------------------------------

/// @brief	write to json
nlohmann::ordered_json Generator::Write() const
{
    nlohmann::ordered_json data;

    data["Radius"] = m_PowerRadius;
    data["ActivateRadius"] = m_ActivationRadius;
    data["Depth"] = m_Depth;
    data["Active"] = m_IsActive;
    data["GrowthSpeed"] = m_RadiusSpeed;


    return data;
}


