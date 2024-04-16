///*****************************************************************/
/// @file	    Generator.cpp
/// @author     Tyler Birdsall (tyler.birdsall@digipen.edu)
/// @date	    01/12/2024
/// @brief      Generator class implimentation
/// @copyright  Digipen LLC (c) 2024
///*****************************************************************/


#include "pch.h" // precompiled header has to be included first
#include "Generator.h"

#include "ComponentSystem.h"
#include "BehaviorSystem.h"

#include "CollisionSystem.h"
#include "EnemyBehavior.h"

#include "ComponentReference.t.h"

#include "Health.h"
#include "Emitter.h"
#include "EmitterSprite.h"
#include "Interactor.h"
#include "Inventory.h"

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

        m_Collider.SetOnConnectCallback( [ this ]()
        {
            m_Collider->AddOnCollisionEnterCallback(
                GetId(),
                std::bind(&Generator::onCollisionEnter, this, std::placeholders::_1)
            );
        } );
        m_Collider.SetOnDisconnectCallback( [ this ]()
        {
            m_Collider->RemoveOnCollisionEnterCallback( GetId() );
        } );

        m_Emitter.SetOnConnectCallback( [ this ]()
        {
            if ( m_IsActive )
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
        } );

        m_Interactable.SetOnConnectCallback( [ this ]() {
            m_Interactable->SetEnabled( m_IsActive == false );
            m_Interactable->AddOnInteractCallback( GetId(), [ this ]( Interactor*  interactor )
            {
                Inventory* playerInv = interactor->GetEntity()->GetComponent< Inventory >();

                if ( playerInv == nullptr || playerInv->ContainsItemStacks( m_ActivationCost ) == false )
                {
                    return;
                }

                Activate();

                playerInv->RemoveItemStacks( m_ActivationCost );
            } );
        } );
        m_Interactable.SetOnDisconnectCallback( [ this ]()
        {
            m_Interactable->RemoveOnInteractCallback( GetId() );
        } );

        m_Collider        .Init( GetEntity() );
        m_AudioPlayer     .Init( GetEntity() );
        m_Transform       .Init( GetEntity() );
        m_Health          .Init( GetEntity() );
        m_Emitter         .Init( GetEntity() );
        m_Light           .Init( GetEntity() );
        m_Sprite          .Init( GetEntity() );
        m_PathfinderTarget.Init( GetEntity() );
        m_Interactable    .Init( GetEntity() );

        m_WavePrefab     .SetOwnerName( GetName() );
        m_ActivateSound  .SetOwnerName( GetName() );
        m_DeactivateSound.SetOwnerName( GetName() );
        m_DamageSound    .SetOwnerName( GetName() );


        for (auto& reward : m_RewardPrefabs)
        {
			reward.SetOwnerName( GetName() );
			reward.Init();
		}


        m_WavePrefab     .Init( false );
        m_ActivateSound  .Init();
        m_DamageSound    .Init();
        m_DeactivateSound.Init();


        m_ChangeActive = m_IsActive;
        m_CanActivate = !m_IsActive;
    }

    /// @brief	called on exit, handles loss state
    void Generator::OnExit()
    {
        Behaviors< Generator >()->RemoveComponent(this);

        m_Collider        .Exit();
        m_AudioPlayer     .Exit();
        m_Transform       .Exit();
        m_Health          .Exit();
        m_Emitter         .Exit();
        m_Sprite          .Exit();
        m_PathfinderTarget.Exit();
        m_Interactable    .Exit();
        m_Light           .Exit();
    }

    /// @brief  called every frame
    /// @param  dt delta time
    void Generator::OnUpdate(float dt)
    {
        if (m_IsActive != m_ChangeActive)
        {
            if (m_ChangeActive == true)
            {
                Activate();
            }
            else
            {
                Deactivate();
            }
        }

        if ( m_Emitter == nullptr )
        {
            return;
        }

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

            if (m_AudioPlayer)
            {
                m_AudioPlayer->SetSound(m_ActivateSound);
                m_AudioPlayer->Play();
            }
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

            if (m_AudioPlayer)
            {
                m_AudioPlayer->Stop(); // Stop the hit sound
                m_AudioPlayer->SetSound(m_DeactivateSound); // load the deactivate sound
                m_AudioPlayer->Play(); // play the deactivate sound
            }
        }
        else if (m_ShrinkRing)
        {
            m_GrowthRadius -= m_RadiusSpeed * dt;
            if (m_GrowthRadius >= m_PowerRadius)
            {
                m_GrowthRadius = m_PowerRadius;
                m_ShrinkRing = false;
            }
            ParticleSystem::EmitData data = m_Emitter->GetEmitData();
            data.startAhead = m_GrowthRadius;
            m_Emitter->SetEmitData(data);
            m_Emitter->SetContinuous(true);
        }
    }

//-----------------------------------------------------------------------------
// copying
//-----------------------------------------------------------------------------


    /// @brief  copy ctor
    Generator::Generator(const Generator& other) :
        Behavior( other ),
        m_IsActive       ( other.m_IsActive        ),
        m_PowerRadius    ( other.m_PowerRadius     ),
        m_RadiusSpeed    ( other.m_RadiusSpeed     ),
        m_WavePrefab     ( other.m_WavePrefab      ),
        m_ActivationCost ( other.m_ActivationCost  ),
        m_ActivateSound  ( other.m_ActivateSound   ),
        m_DeactivateSound( other.m_DeactivateSound ),
        m_DamageSound    ( other.m_DamageSound     ),
        m_RewardPrefabs  ( other.m_RewardPrefabs   ),
        m_CanBeRewarded  ( other.m_CanBeRewarded   )
    {}


//-----------------------------------------------------------------------------
// accessors
//-----------------------------------------------------------------------------


    /// @brief returns the radius within turrets are powered
    /// @return the power radius of the generator
    float Generator::GetPowerRadius() const
    {
        return m_PowerRadius;
    }

    /// @brief returns if the generator is powered or not
    /// @return is the generator active
    bool Generator::GetActive() const
    {
        return m_IsActive;
    }

    /// @brief  gets the activation cost of this Generator
    /// @return the activation cost of this Generator
    std::vector< ItemStack > const& Generator::GetActivationCost() const
    {
        return m_ActivationCost;
    }

    /// @brief  get the transform of the generator
    /// @return the generator transform
    Transform const* Generator::GetTransform() const
    {
        return m_Transform;
    }



//-----------------------------------------------------------------------------
// public: methods
//-----------------------------------------------------------------------------


    /// @brief activate the generator
    void Generator::Activate() 
    {
        if ( m_IsActive )
        {
            return;
        }

        if (m_WavePrefab != nullptr && m_CanSpawnWave)
        {
            Entity* wave = m_WavePrefab->Clone();
            wave->AddToScene();
            m_CanSpawnWave = false;
        }

        m_IsActive = true;
        m_CanActivate = false;
        m_ChangeActive = true;
        m_ActivateRing = true;

        if (m_Sprite != nullptr)
        {
            m_Sprite->SetFrameIndex(1);
        }

        if ( m_PathfinderTarget != nullptr )
        {
            m_PathfinderTarget->SetActive( true );
        }

        if ( m_Interactable != nullptr )
        {
            m_Interactable->SetEnabled( false );
        }

        if (m_Light != nullptr)
        {
            m_Light->SetStrength(0.8f);
        }
    }

    /// @brief deactivate the generator
    void Generator::Deactivate()
    {
        m_IsActive = false;
        m_CanActivate = true;
        m_ChangeActive = false;
        m_DeactivateRing = true;
        m_CanSpawnWave = true;

        if ( m_Interactable != nullptr )
        {
            m_Interactable->SetEnabled( true );
        }
    }


//-----------------------------------------------------------------------------
// private functions
//-----------------------------------------------------------------------------

    /// @brief collision callback for generators
    void Generator::onCollisionEnter(Collider* other)
    {
        if ( m_IsActive == false )
        {
            return;
        }

        if ( m_Health == nullptr )
        {
            return;
        }

        EnemyBehavior* enemy = other->GetEntity()->GetComponent<EnemyBehavior>();
        if (!enemy)
        {
            return;
        }

        m_Health->TakeDamage( enemy->GetDamage() );

        if ( m_AudioPlayer )
        {
            m_AudioPlayer->SetSound(m_DamageSound);
            m_AudioPlayer->Play();
        }
        enemy->GetEntity()->Destroy();

        if (m_Health->GetHealth()->GetCurrent() <= 0)
        {
            Deactivate();
            m_Health->GetHealth()->Reset();

            if ( m_PathfinderTarget != nullptr )
            {
                m_PathfinderTarget->SetActive( false );
            }
        }
    }

//-----------------------------------------------------------------------------
// inspector methods
//-----------------------------------------------------------------------------

    /// @brief	inspector for generators
    void Generator::Inspector()
    {
        ImGui::DragFloat( "Radius"      , &m_PowerRadius, 0.05f, 0.0f, INFINITY );
        ImGui::DragFloat( "Growth Speed", &m_RadiusSpeed, 0.05f, 0.0f, INFINITY );
        m_WavePrefab.Inspect( "Wave to Spawn" );
        m_ActivateSound.Inspect( "Activate Sound" );
        m_DeactivateSound.Inspect( "Deactivate Sound" );
        m_DamageSound.Inspect( "Damage Sound" );


        ImGui::Checkbox( "Is Active", &m_ChangeActive );

        Inspection::InspectArray< ItemStack >( "Activation cost", &m_ActivationCost, []( ItemStack* itemStack ) -> bool
        {
            return itemStack->Inspect();
        } );


        Inspection::InspectArray< AssetReference< Entity > >("Rewards",
         &m_RewardPrefabs, [](AssetReference< Entity >* reward) -> bool
        {
			return reward->Inspect("Reward to Spawn");
		});
    }

//-----------------------------------------------------------------------------
// reading
//-----------------------------------------------------------------------------

    /// @brief read method map
    ReadMethodMap<Generator> const Generator::s_ReadMethods =
    {
        { "Radius"         , &readRadius          },
        { "Active"         , &readActive          },
        { "GrowthSpeed"    , &readSpeed           },
        { "WavePrefab"     , &readWavePrefab      },
        { "ActivationCost" , &readActivationCost  },
        { "ActivateSound"  , &readActivateSound   },
        { "DeactivateSound", &readDeactivateSound },
        { "DamageSound"    , &readDamageSound     },
		{ "Rewards"        , &readRewardPrefabs   },
		{ "CanBeRewarded"  , &readCanBeRewarded   }
    };

    /// @brief	read the raidus from json
    void Generator::readRadius(nlohmann::ordered_json const& json)
    {
        m_PowerRadius = Stream::Read<float>(json);
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

    /// @brief	read the attached wave prefab to spawn
    void Generator::readWavePrefab(nlohmann::ordered_json const& json)
    {
        Stream::Read(m_WavePrefab, json);
    }

    void Generator::readActivateSound(nlohmann::ordered_json const& json)
    {
        Stream::Read(m_ActivateSound, json);
    }

    void Generator::readDeactivateSound(nlohmann::ordered_json const& json)
    {
        Stream::Read(m_DeactivateSound, json);
    }

    void Generator::readDamageSound(nlohmann::ordered_json const& json)
    {
            Stream::Read(m_DamageSound, json);
    }

    void Generator::readActivationCost(nlohmann::ordered_json const& json)
    {
        Stream::Read< ItemStack >( &m_ActivationCost, json );
    }

    void Generator::readRewardPrefabs(nlohmann::ordered_json const& json)
    {
		Stream::Read< AssetReference< Entity > >( &m_RewardPrefabs, json );
    }

    void Generator::readCanBeRewarded(nlohmann::ordered_json const& json)
    {
        Stream::Read(m_CanBeRewarded, json);
    }

//-----------------------------------------------------------------------------
// writing
//-----------------------------------------------------------------------------

    /// @brief	write to json
    nlohmann::ordered_json Generator::Write() const
    {
        nlohmann::ordered_json data;

        data[ "Radius"         ] = Stream::Write( m_PowerRadius );
        data[ "Active"         ] = Stream::Write( m_IsActive    );
        data[ "GrowthSpeed"    ] = Stream::Write( m_RadiusSpeed );
        data[ "WavePrefab"     ] = Stream::Write( m_WavePrefab  );
        data[ "ActivateSound"  ] = Stream::Write( m_ActivateSound );
        data[ "DeactivateSound"] = Stream::Write( m_DeactivateSound );
        data[ "DamageSound"    ] = Stream::Write( m_DamageSound );
        data[ "ActivationCost" ] = Stream::WriteArray( m_ActivationCost );

        data[ "Rewards" ] = Stream::WriteArray(m_RewardPrefabs);

        return data;
    }

//-----------------------------------------------------------------------------
