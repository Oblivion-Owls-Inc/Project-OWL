/// @file       ResourceCounterUi.cpp
/// @author     Steve Bukowinski (steve.bukowinski@digipen.edu)
/// @brief      Ui Component that displays a resource sprite and a count
/// @version    0.1
/// @date       2024-02-21
/// 
/// @copyright  Copyright (c) 2024 Digipen Institute of Technology

#include "pch.h" // precompiled header has to be included first
#include "ResourceCounterUi.h"

#include "ComponentReference.t.h"
#include "Sprite.h"
#include "Text.h"
#include "UiElement.h"
#include "TilemapSprite.h"


//-----------------------------------------------------------------------------
// public: constructor / Destructor
//-----------------------------------------------------------------------------


    /// @brief  default constructor
    ResourceCounterUi::ResourceCounterUi() :
        Component( typeid( ResourceCounterUi ) )
    {}


//-----------------------------------------------------------------------------
// public: methods
//-----------------------------------------------------------------------------


    /// @brief  sets the resources this ResourceCounter should display
    /// @param  itemStack   the ItemStack of resources to display
    void ResourceCounterUi::SetResources( ItemStack const& itemStack )
    {
        m_Resources = itemStack;

        if ( m_Sprite != nullptr )
        {
            m_Sprite->SetFrameIndex( itemStack.M_ItemId );
        }

        if ( m_Text != nullptr )
        {
            m_Text->SetText( std::to_string( itemStack.M_Count ) );
        }
    }


    /// @brief  sets the opacity of this ResourceCounter
    /// @param  opacity what the opacity of this ResourceCounter to be
    void ResourceCounterUi::SetOpacity( float opacity )
    {
        m_Opacity = opacity;

        if ( m_Sprite != nullptr )
        {
            m_Sprite->SetOpacity( opacity );
        }

        if ( m_TilemapSprite != nullptr )
        {
            m_TilemapSprite->SetOpacity( opacity );
        }
    }

//-----------------------------------------------------------------------------
// public: accessors
//-----------------------------------------------------------------------------


    /// @brief  gets the itemId of the resource this ResourceCounter displays
    /// @return the itemId of the resource this ResourceCounter displays
    int ResourceCounterUi::GetResourceId() const
    {
        return m_Resources.M_ItemId;
    }

    /// @brief  gets the ItemStack this ResourceCounter represents
    /// @return the ItemStack this ResourceCounter represents
    ItemStack const& ResourceCounterUi::GetItemStack() const
    {
        return m_Resources;
    }


    /// @brief  gets the UiElement attached to this ResourceCounterUi
    /// @return the UiElement attached to this ResourceCounterUi
    UiElement* ResourceCounterUi::GetUiElement()
    {
        if ( m_UiElement == nullptr )
        {
            m_UiElement.Init( GetEntity() );
        }
        return m_UiElement;
    }

    /// @brief  gets the Sprite attached to this ResourceCounterUi
    /// @return the Sprite attached to this ResourceCounterUi
    Sprite* ResourceCounterUi::GetSprite()
    {
        if ( m_Sprite == nullptr )
        {
            m_Sprite.Init( GetEntity() );
        }
        return m_Sprite;
    }

    /// @brief  gets the Text Sprite attached to this ResourceCounterUi
    /// @return the Text Sprite attached to this ResourceCounterUi
    TilemapSprite* ResourceCounterUi::GetTextSprite()
    {
        return m_TilemapSprite;
    }

    /// @brief  gets the Text attached to this ResourceCounterUi
    /// @return the Text attached to this ResourceCounterUi
    Text* ResourceCounterUi::GetText()
    {
        if ( m_Text == nullptr )
        {
            m_Text.Init( GetEntity() );
        }
        return m_Text;
    }


//-----------------------------------------------------------------------------
// public: virtual override methods
//-----------------------------------------------------------------------------


    /// @brief  called once when entering the scene
    void ResourceCounterUi::OnInit()
    {
        m_Sprite.SetOnConnectCallback(
            [ this ]()
            {
                m_Sprite->SetFrameIndex( m_Resources.M_ItemId );
                m_Sprite->SetOpacity( m_Opacity );
            }
        );
        m_Text.SetOnConnectCallback(
            [ this ]()
            {
                m_Text->SetText( std::to_string( m_Resources.M_Count ) );
            }
        );
        m_TilemapSprite.SetOnConnectCallback(
            [ this ]()
            {
                m_TilemapSprite->SetOpacity( m_Opacity );
            }
        );

        m_UiElement    .Init( GetEntity() );
        m_Sprite       .Init( GetEntity() );

        if ( GetEntity()->GetChildren().empty() == false )
        {
            m_Text         .Init( GetEntity()->GetChildren()[ 0 ] );
            m_TilemapSprite.Init( GetEntity()->GetChildren()[ 0 ] );
        }
    }

    /// @brief  called once when exiting the scene
    void ResourceCounterUi::OnExit()
    {
        m_UiElement    .Exit();
        m_Sprite       .Exit();
        m_Text         .Exit();
        m_TilemapSprite.Exit();
    }

    
    /// @brief  called after a Child Entity is added
    /// @param  child   the Child Entity that was added
    void ResourceCounterUi::OnAddChild( Entity* child )
    {
        if ( m_Text.GetEntity() == nullptr )
        {
            m_Text.Init( child );
        }
    }

    /// @brief  called before a Child Entity is removed
    /// @param  child   the Child Entity that will be removed
    void ResourceCounterUi::OnRemoveChild( Entity* child )
    {
        if ( m_Text.GetEntity() == child )
        {
            m_Text.Exit();
        }
    }


//-----------------------------------------------------------------------------
// private: methods
//-----------------------------------------------------------------------------


//-----------------------------------------------------------------------------
// public: inspection
//-----------------------------------------------------------------------------


    /// @brief  shows the inspector for ResourceCounterUi
    void ResourceCounterUi::Inspector()
    {
        ImGui::Text( "resources to display" );
        if ( m_Resources.Inspect() )
        {
            SetResources( m_Resources );
        }
    }


//-----------------------------------------------------------------------------
// private: reading
//-----------------------------------------------------------------------------


    /// @brief  reads the resources this ResourceCounterUi displays
    /// @param  data    the JSON data to read from
    void ResourceCounterUi::readResources( nlohmann::ordered_json const& data )
    {
        Stream::Read( m_Resources, data );
    }

    /// @brief  reads the opacity of this ResourceCounter
    /// @param  data    the JSON data to read from
    void ResourceCounterUi::readOpacity( nlohmann::ordered_json const& data )
    {
        Stream::Read( m_Opacity, data );
    }


//-----------------------------------------------------------------------------
// public: reading / writing
//-----------------------------------------------------------------------------

    
    /// @brief  gets the map of read methods for this ResourceCounterUi
    /// @return the map of read methods for this ResourceCounterUi
    ReadMethodMap< ISerializable > const& ResourceCounterUi::GetReadMethods() const
    {
        static ReadMethodMap< ResourceCounterUi > const readMethods = {
            { "Resources", &ResourceCounterUi::readResources },
            { "Opacity"  , &ResourceCounterUi::readOpacity   }
        };

        return (ReadMethodMap< ISerializable > const&)readMethods;
    }


    /// @brief  writes this ResourceCounterUi to JSON
    /// @return the JSON data of this ResourceCounterUi
    nlohmann::ordered_json ResourceCounterUi::Write() const
    {
        nlohmann::ordered_json json;

        json[ "Resources" ] = Stream::Write( m_Resources );
        json[ "Opacity"   ] = Stream::Write( m_Opacity   );

        return json;
    }


//-----------------------------------------------------------------------------
// public: copying
//-----------------------------------------------------------------------------


    /// @brief  clones this ResourceCounterUi
    /// @return the newly created clone of this ResourceCounterUi
    ResourceCounterUi* ResourceCounterUi::Clone() const
    {
        return new ResourceCounterUi( *this );
    }


//-----------------------------------------------------------------------------
// private: copying
//-----------------------------------------------------------------------------


    /// @brief  copy-constructor for the ResourceCounterUi
    /// @param  other   the other ResourceCounterUi to copy
    ResourceCounterUi::ResourceCounterUi( ResourceCounterUi const& other ) :
        Component( other ),
        m_Resources( other.m_Resources ),
        m_Opacity  ( other.m_Opacity   )
    {}


//-----------------------------------------------------------------------------
