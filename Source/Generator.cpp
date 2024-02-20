///*****************************************************************/
/// @file	    Generator.cpp
/// @author     Tyler Birdsall (tyler.birdsall@digipen.edu)
/// @date	    01/12/2024
/// @brief      Generator class implimentation
/// @copyright  Digipen LLC (c) 2024
///*****************************************************************/

#include "Generator.h"

#include "ComponentSystem.h"

#include "CollisionSystem.h"
#include "EnemyBehavior.h"

#include "Health.h"

//-----------------------------------------------------------------------------
// constructor / destructor 
//-----------------------------------------------------------------------------

/// @brief  constructor
Generator::Generator() :
    Component( typeid( Generator ) )
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
    Components< Generator >()->AddComponent(this);

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

}

/// @brief	called on exit, handles loss state
void Generator::OnExit()
{
    Components< Generator >()->RemoveComponent(this);

    m_Collider   .Exit( GetEntity() );
    m_AudioPlayer.Exit( GetEntity() );
    m_Transform  .Exit( GetEntity() );
    m_Health     .Exit( GetEntity() );
}

//-----------------------------------------------------------------------------
// copying
//-----------------------------------------------------------------------------

/// @brief  copy ctor
Generator::Generator(const Generator& other) :
    Component( other ),
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


    return data;
}


