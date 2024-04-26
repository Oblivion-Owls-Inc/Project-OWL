/// @file       GeneratorActivationCostUi.cpp
/// @author     Steve Bukowinski (steve.bukowinski@digipen.edu)
/// @brief      displays the costs of activating generators
/// @version    0.1
/// @date       April 2024
/// 
/// @copyright  Copyright (c) 2024 Digipen Institute of Technology

#include "pch.h"

#include "GeneratorActivationCostUi.h"

#include "BehaviorSystem.h"

#include "ComponentReference.t.h"
#include "Interactor.h"
#include "Inventory.h"
#include "ResourcesUiManager.h"

#include "Generator.h"

#include "UiElement.h"
#include "CameraSystem.h"



//-----------------------------------------------------------------------------
// public: constructor / Destructor
//-----------------------------------------------------------------------------


    /// @brief  default constructor
    GeneratorActivationCostUi::GeneratorActivationCostUi() :
        Behavior( typeid( GeneratorActivationCostUi ) )
    {}


//-----------------------------------------------------------------------------
// public: methods
//-----------------------------------------------------------------------------


//-----------------------------------------------------------------------------
// public: accessors
//-----------------------------------------------------------------------------


//-----------------------------------------------------------------------------
// public: virtual override methods
//-----------------------------------------------------------------------------


    /// @brief  called once when entering the scene
    void GeneratorActivationCostUi::OnInit()
    {
        Behaviors< Behavior >()->AddComponent( this );

        m_Interactor.Init( GetEntity() );
        m_Inventory .Init( GetEntity() );

        m_ResourcesUiEntity.SetOwnerName( GetName() );
        m_ResourcesUiEntity.Init();
    }

    /// @brief  called once when exiting the scene
    void GeneratorActivationCostUi::OnExit()
    {
        Behaviors< Behavior >()->RemoveComponent( this );

        m_Interactor.Exit();
        m_Inventory .Exit();

        m_ResourcesUiEntity.Exit();
    }


    /// @brief  called once every simulation frame
    void GeneratorActivationCostUi::OnFixedUpdate()
    {
        if (
            m_Interactor         == nullptr ||
            m_Inventory          == nullptr ||
            m_ResourcesUiManager == nullptr ||
            m_ResourcesUiManager->GetInventory() == nullptr ||
            m_ResourcesUiManager->GetUiElement() == nullptr
        )
        {
            return;
        }

        Interactable* targetedInteractable = m_Interactor->GetTargetedInteractable();

        if ( targetedInteractable == nullptr )
        {
            m_ResourcesUiManager->SetOpacity( 0.0f );
            return;
        }

        Generator const* generator = targetedInteractable->GetEntity()->GetComponent< Generator >();
        if ( generator == nullptr || generator->GetTransform() == nullptr )
        {
            m_ResourcesUiManager->SetOpacity( 0.0f );
            return;
        }

        m_ResourcesUiManager->SetOpacity( 1.0f );

        glm::vec2 targetPos = generator->GetTransform()->GetTranslation();
        m_ResourcesUiManager->GetUiElement()->SetAnchor(
            Cameras()->GetMat_WorldToClip() * glm::vec4( targetPos + m_CostUiOffset, 0.0f, 1.0f )
        );

        m_ResourcesUiManager->SetTextColors( glm::vec4( 0.0f, 0.0f, 0.0f, 0.0f ) );
        m_ResourcesUiManager->SetTextColors( glm::vec4( 0.0f, -1.0f, -1.0f, 0.0f ), [ this ]( ItemStack const& itemStack ) -> bool
        {
            return m_Inventory->ContainsItemStack( itemStack ) == false;
        } );

        if ( m_Interactor->GetTargetedInteractable() == m_TargetedInteractable )
        {
            return;
        }

        m_TargetedInteractable = m_Interactor->GetTargetedInteractable();

        m_ResourcesUiManager->GetInventory()->Clear();
        m_ResourcesUiManager->GetInventory()->AddItemStacks( generator->GetActivationCost() );

    }


//-----------------------------------------------------------------------------
// private: methods
//-----------------------------------------------------------------------------


//-----------------------------------------------------------------------------
// public: inspection
//-----------------------------------------------------------------------------


    /// @brief  shows the inspector for GeneratorActivationCostUi
    void GeneratorActivationCostUi::Inspector()
    {
        ImGui::DragFloat2( "offset", &m_CostUiOffset[ 0 ], 0.05f );

        m_ResourcesUiEntity.Inspect( "Resources Ui Entity" );
    }


//-----------------------------------------------------------------------------
// private: reading
//-----------------------------------------------------------------------------


    /// @brief  read sthe offset from the Interactable's Transform of the cost UI
    /// @param  data    the JSON data to read from
    void GeneratorActivationCostUi::readCostUiOffset( nlohmann::ordered_json const& data )
    {
        Stream::Read( &m_CostUiOffset, data );
    }


    /// @brief  reads the Entity the ResourcesUiManager is attached to
    /// @param  data    the JSON data to read from
    void GeneratorActivationCostUi::readResourcesUiEntity( nlohmann::ordered_json const& data )
    {
        Stream::Read( m_ResourcesUiEntity, data );
    }


//-----------------------------------------------------------------------------
// public: reading / writing
//-----------------------------------------------------------------------------


    /// @brief  gets the map of read methods for this GeneratorActivationCostUi
    /// @return the map of read methods for this GeneratorActivationCostUi
    ReadMethodMap< ISerializable > const& GeneratorActivationCostUi::GetReadMethods() const
    {
        static ReadMethodMap< GeneratorActivationCostUi > const readMethods = {
            { "CostUiOffset"     , &GeneratorActivationCostUi::readCostUiOffset      },
            { "ResourcesUiEntity", &GeneratorActivationCostUi::readResourcesUiEntity }
        };

        return (ReadMethodMap< ISerializable > const&)readMethods;
    }


    /// @brief  writes this GeneratorActivationCostUi to JSON
    /// @return the JSON data of this GeneratorActivationCostUi
    nlohmann::ordered_json GeneratorActivationCostUi::Write() const
    {
        nlohmann::ordered_json json = nlohmann::ordered_json::object();

        json[ "CostUiOffset"      ] = Stream::Write( m_CostUiOffset      );
        json[ "ResourcesUiEntity" ] = Stream::Write( m_ResourcesUiEntity );

        return json;
    }

    
//-----------------------------------------------------------------------------
// public: copying
//-----------------------------------------------------------------------------


    /// @brief  clones this GeneratorActivationCostUi
    /// @return the newly created clone of this GeneratorActivationCostUi
    GeneratorActivationCostUi* GeneratorActivationCostUi::Clone() const
    {
        return new GeneratorActivationCostUi( *this );
    }


//-----------------------------------------------------------------------------
// private: copying
//-----------------------------------------------------------------------------


    /// @brief  copy-constructor for the GeneratorActivationCostUi
    /// @param  other   the other GeneratorActivationCostUi to copy
    GeneratorActivationCostUi::GeneratorActivationCostUi( GeneratorActivationCostUi const& other ) :
        Behavior( other ),
        m_CostUiOffset     ( other.m_CostUiOffset ),
        m_ResourcesUiEntity( other.m_ResourcesUiEntity, { &m_ResourcesUiManager } )
    {}


//-----------------------------------------------------------------------------
