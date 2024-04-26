/// @file       BuildingSelectionButton.cpp
/// @author     Steve Bukowinski (steve.bukowinski@digipen.edu)
/// @brief      button that selects which building the ConstructionManager should place
/// @version    0.1
/// @date       DATE HERE
/// 
/// @copyright  Copyright (c) 2024 Digipen Institute of Technology

#include "pch.h"

#include "BuildingSelectionButton.h"

#include "ComponentReference.t.h"
#include "UiButton.h"


#include "ConstructionBehavior.h"
#include "Inventory.h" // needed to prevent compilation errors from ConstructionBehavior

#include "ResourcesUiManager.h"
#include "Popup.h"

#include "BehaviorSystem.h"


//-----------------------------------------------------------------------------
// public: constructor / Destructor
//-----------------------------------------------------------------------------


    /// @brief  default constructor
    BuildingSelectionButton::BuildingSelectionButton() :
        Behavior( typeid( BuildingSelectionButton ) )
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
    void BuildingSelectionButton::OnInit()
    {
        Behaviors< Behavior >()->AddComponent( this );

        m_UiButton.SetOnConnectCallback( [ this ]() {
            m_UiButton->AddOnClickedCallback( GetId(), [ this ]() {
                if ( m_ConstructionBehavior != nullptr && m_ConstructionBehavior->BuildingIsUnlocked( m_BuildingIndex ) )
                {
                    m_ConstructionBehavior->SetBuildingIndex( m_BuildingIndex );
                }
            } );
        } );
        m_UiButton.SetOnDisconnectCallback( [ this ]() {
            m_UiButton->RemoveOnOnClickedCallback( GetId() );
        } );

        m_UiButton.Init( GetEntity() );
        m_Sprite  .Init( GetEntity() );

        m_UnselectedTexture.SetOwnerName( GetName() );
        m_SelectedTexture  .SetOwnerName( GetName() );
        m_LockedTexture    .SetOwnerName( GetName() );
        m_UnselectedTexture.Init();
        m_SelectedTexture  .Init();
        m_LockedTexture    .Init();

        m_ConstructionEntity.SetOwnerName( GetName() );
        m_ConstructionEntity.Init();
    }

    /// @brief  called once when exiting the scene
    void BuildingSelectionButton::OnExit()
    {
        Behaviors< Behavior >()->RemoveComponent( this );

        m_UiButton.Exit();
        m_Sprite  .Exit();

        m_ConstructionEntity.Exit();
    }


    /// @brief  called every simulation frame
    void BuildingSelectionButton::OnFixedUpdate()
    {
        if ( m_ConstructionBehavior == nullptr || m_Sprite == nullptr )
        {
            return;
        }

        if ( m_ConstructionBehavior->BuildingIsUnlocked( m_BuildingIndex ) == false )
        {
            m_Sprite->SetTexture( m_LockedTexture );
        }
        else if ( m_ConstructionBehavior->GetBuildingIndex() == m_BuildingIndex )
        {
            m_Sprite->SetTexture( m_SelectedTexture );
        }
        else
        {
            m_Sprite->SetTexture( m_UnselectedTexture );
        }
    }


//-----------------------------------------------------------------------------
// private: methods
//-----------------------------------------------------------------------------


//-----------------------------------------------------------------------------
// public: inspection
//-----------------------------------------------------------------------------


    /// @brief  shows the inspector for BuildingSelectionButton
    void BuildingSelectionButton::Inspector()
    {
        ImGui::DragInt( "building index", &m_BuildingIndex, 0.05f, 0, INT_MAX );

        ImGui::NewLine();

        m_UnselectedTexture.Inspect( "unselected texture" );
        m_SelectedTexture  .Inspect( "selected texture"   );
        m_LockedTexture    .Inspect( "locked texture"     );

        ImGui::NewLine();

        m_ConstructionEntity.Inspect( "Construction Entity" );
    }


//-----------------------------------------------------------------------------
// private: reading
//-----------------------------------------------------------------------------


    /// @brief  reads the building index that this button selects
    /// @param  data    the JSON data to read from
    void BuildingSelectionButton::readBuildingIndex( nlohmann::ordered_json const& data )
    {
        Stream::Read( m_BuildingIndex, data );
    }


    /// @brief  reads the texture to display when this button is not selected
    /// @param  data    the JSON data to read from
    void BuildingSelectionButton::readUnselectedTexture( nlohmann::ordered_json const& data )
    {
        Stream::Read( m_UnselectedTexture, data );
    }

    /// @brief  reads the texture to display when this button is selected
    /// @param  data    the JSON data to read from
    void BuildingSelectionButton::readSelectedTexture( nlohmann::ordered_json const& data )
    {
        Stream::Read( m_SelectedTexture, data );
    }

    /// @brief  reads the texture to display when this button is locked
    /// @param  data    the JSON data to read from
    void BuildingSelectionButton::readLockedTexture( nlohmann::ordered_json const& data )
    {
        Stream::Read( m_LockedTexture, data );
    }


    /// @brief  reads the ConstructionManager Entity
    /// @param  data    the JSON data to read from
    void BuildingSelectionButton::readConstructionEntity( nlohmann::ordered_json const& data )
    {
        Stream::Read( m_ConstructionEntity, data );
    }


//-----------------------------------------------------------------------------
// public: reading / writing
//-----------------------------------------------------------------------------


    /// @brief  gets the map of read methods for this BuildingSelectionButton
    /// @return the map of read methods for this BuildingSelectionButton
    ReadMethodMap< ISerializable > const& BuildingSelectionButton::GetReadMethods() const
    {
        static ReadMethodMap< BuildingSelectionButton > const readMethods = {
            { "BuildingIndex"     , &BuildingSelectionButton::readBuildingIndex      },
            { "UnselectedTexture" , &BuildingSelectionButton::readUnselectedTexture  },
            { "SelectedTexture"   , &BuildingSelectionButton::readSelectedTexture    },
            { "LockedTexture"     , &BuildingSelectionButton::readLockedTexture      },
            { "ConstructionEntity", &BuildingSelectionButton::readConstructionEntity }
        };

        return (ReadMethodMap< ISerializable > const&)readMethods;
    }


    /// @brief  writes this BuildingSelectionButton to JSON
    /// @return the JSON data of this BuildingSelectionButton
    nlohmann::ordered_json BuildingSelectionButton::Write() const
    {
        nlohmann::ordered_json json = nlohmann::ordered_json::object();

        json[ "BuildingIndex"      ] = Stream::Write( m_BuildingIndex      );
        json[ "UnselectedTexture"  ] = Stream::Write( m_UnselectedTexture  );
        json[ "SelectedTexture"    ] = Stream::Write( m_SelectedTexture    );
        json[ "LockedTexture"      ] = Stream::Write( m_LockedTexture      );
        json[ "ConstructionEntity" ] = Stream::Write( m_ConstructionEntity );

        return json;
    }

    
//-----------------------------------------------------------------------------
// public: copying
//-----------------------------------------------------------------------------


    /// @brief  clones this BuildingSelectionButton
    /// @return the newly created clone of this BuildingSelectionButton
    BuildingSelectionButton* BuildingSelectionButton::Clone() const
    {
        return new BuildingSelectionButton( *this );
    }


//-----------------------------------------------------------------------------
// private: copying
//-----------------------------------------------------------------------------


    /// @brief  copy-constructor for the BuildingSelectionButton
    /// @param  other   the other BuildingSelectionButton to copy
    BuildingSelectionButton::BuildingSelectionButton( BuildingSelectionButton const& other ) :
        Behavior( other ),
        m_BuildingIndex     ( other.m_BuildingIndex      ),
        m_UnselectedTexture ( other.m_UnselectedTexture  ),
        m_SelectedTexture   ( other.m_SelectedTexture    ),
        m_LockedTexture     ( other.m_LockedTexture      ),
        m_ConstructionEntity( other.m_ConstructionEntity, { &m_ConstructionBehavior } )
    {}


//-----------------------------------------------------------------------------
