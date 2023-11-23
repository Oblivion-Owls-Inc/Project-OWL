/// @file       UiElement.cpp
/// @author     Steve Bukowinski (steve.bukowinski@digipen.edu)
/// @brief      Ui element component
/// @version    0.1
/// @date       2023-11-03
/// 
/// @copyright  Copyright (c) 2023 Digipen Institute of Technology

//-----------------------------------------------------------------------------
// includes
//-----------------------------------------------------------------------------

#include "UiElement.h"

#include "EntitySystem.h"
#include "DebugSystem.h"
#include "CameraSystem.h"
#include "RenderSystem.h"
#include "PlatformSystem.h"

#include "Inspection.h"

//-----------------------------------------------------------------------------
// public: constructor
//-----------------------------------------------------------------------------


    /// @brief  constructor
    UiElement::UiElement() :
        Transform( typeid( UiElement ) )
    {
        SetIsDiegetic( false );
    }


//-----------------------------------------------------------------------------
// public: methods
//-----------------------------------------------------------------------------
    

    /// @brief  adds a child to this UiElement
    /// @param  child   the child to add
    void UiElement::AddChild( UiElement* child )
    {
        if ( child->m_Parent != nullptr )
        {
            child->m_Parent->RemoveChild( child );
        }

        m_Children.push_back( child );

        child->m_Parent = this;
        child->m_ParentName = GetEntity()->GetName();
        child->updateTransform();
    }

    /// @brief  removes a child from this UiElement
    /// @param  child   the child to remove
    void UiElement::RemoveChild( UiElement* child )
    {
        m_Children.erase(
            std::find( m_Children.begin(), m_Children.end(), child )
        );

        child->m_Parent = nullptr;
        child->m_ParentName = "";
        child->updateTransform();
    }


    /// @brief  adds a callback for whenever this UiElement gets clicked
    /// @param  ownerId     the ID of the owner of the callback
    /// @param  callback    the callback to call
    void UiElement::AddOnClickedCallback( unsigned ownerId, std::function< void() > callback )
    {
        // TODO: if callbacks empty, add callback to InputSystem
        m_OnClickedCallbacks.emplace( ownerId, callback );
    }

    /// @brief  removes a callback for whenever this UiElement gets clicked
    /// @param  ownerId     the ID of the owner of the callback
    void UiElement::RemoveOnClickedCallback( unsigned ownerId )
    {
        m_OnClickedCallbacks.erase( ownerId );
        // TODO: if callbacks empty, remove callback from InputSystem
    }


//-----------------------------------------------------------------------------
// public: virtual override methods
//-----------------------------------------------------------------------------

    
    /// @brief  called once when entering the scene
    void UiElement::OnInit()
    {
        Platform()->AddOnWindowResizeCallback( GetId(), std::bind( &UiElement::onWindowResizedCallback, this, std::placeholders::_1 ) );

        if ( m_ParentName.empty() == false )
        {
            Entities()->GetEntity( m_ParentName )->GetComponent< UiElement >()->AddChild( this );
        }
        else
        {
            m_Parent = nullptr;
            updateTransform();
        }
    }

    /// @brief  called once when leaving the scene
    void UiElement::OnExit()
    {
        Platform()->RemoveOnWindowResizeCallback(GetId());

        for ( UiElement* child : m_Children )
        {
            child->m_Parent = nullptr;
            child->updateTransform();
        }
        m_Children.clear();

        if ( m_Parent != nullptr )
        {
            m_Parent->RemoveChild( this );
        }
    }


    /// @brief  displays the Inspector
    void UiElement::Inspector()
    {
        Renderer()->DrawRect( GetTranslation(), GetScale(), 0.0f, glm::vec4( -0.9f, -0.9f, -0.9f, 0.0f ), 0.2f, false );
        
        if ( ImGui::DragFloat2( "Anchor", &m_Anchor[0], 0.01f, -1.0f, 1.0f ) )
        {
            updateTransform();
        }

        if ( ImGui::DragFloat2( "Pivot", &m_Pivot[0], 0.01f, -1.0f, 1.0f ) )
        {
            updateTransform();
        }
        
        if ( ImGui::DragFloat2( "Offset", &m_Offset[0], 0.05f ) )
        {
            updateTransform();
        }

        if ( ImGui::DragFloat2( "Size", &m_FrameSize[0], 0.01f ) )
        {
            updateTransform();
        }

        char const* SizeTypeNames[ 2 ] = { "", "" };
        float width = ImGui::GetColumnWidth() / 3.0f;
        for ( int i = 0; i < 2; ++i )
        {
            for ( auto const& [ name, sizeType ] : s_SizeTypes )
            {
                if ( sizeType == m_SizeTypes[ i ] )
                {
                    SizeTypeNames[ i ] = name.c_str();
                    break;
                }
            }

            ImGui::PushID( i );
            ImGui::PushItemWidth( width );
            if ( ImGui::BeginCombo( "", SizeTypeNames[ i ] ) )
            {
                for ( auto const& [ name, sizeType ] : s_SizeTypes )
                {
                    if ( ImGui::Selectable( name.c_str(), m_SizeTypes[ i ] == sizeType ) )
                    {
                        m_SizeTypes[ i ] = sizeType;
                        updateTransform();
                    }
                }
                ImGui::EndCombo();
            }
            ImGui::PopID();

            ImGui::SameLine();
        }
        ImGui::Text( "Size Types" );


        Entity* parentEntity = m_Parent ? m_Parent->GetEntity() : nullptr;
        if ( Inspection::SelectEntityFromScene( "Parent", &parentEntity ) )
        {
            UiElement* parentUiElement = parentEntity->GetComponent< UiElement >();
            if ( parentUiElement == nullptr )
            {
                Debug()
                    << "Warning: Entity \"" << parentEntity->GetName()
                    << "\" does not have a UiElement component" << std::endl;
            }
            else
            {
                parentUiElement->AddChild( this );
            }
        }
    }


//-----------------------------------------------------------------------------
// private: methods
//-----------------------------------------------------------------------------


    /// @brief  updates the Transform and all children transforms
    void UiElement::updateTransform()
    {

        glm::vec2 parentScale;
        if ( m_Parent == nullptr )
        {
            glm::mat4 clipToUi = glm::inverse( Cameras()->GetMat_UItoClip() );
            parentScale = clipToUi * glm::vec4( 1, 1, 0, 0 ) * 2.0f;
        }
        else
        {
            parentScale = m_Parent->GetScale();
        }

        // TODO: UiElement::updateTransform
        glm::vec2 worldAnchor = (m_Parent ? m_Parent->GetTranslation() : glm::vec2( 0 )) + 0.5f * m_Anchor * parentScale;

        glm::vec2 worldScale;
        for ( int i = 0; i < worldScale.length(); ++i )
        {
            if ( m_SizeTypes[ i ] == SizeType::Absolute )
            {
                worldScale = m_FrameSize;
            }
            else
            {

                if ( m_SizeTypes[ i ] == SizeType::RelativeToWidth )
                {
                    worldScale[ i ] = parentScale.x * m_FrameSize[ i ];
                }
                else if ( m_SizeTypes[ i ] == SizeType::RelativeToHeight )
                {
                    worldScale[ i ] = parentScale.y * m_FrameSize[ i ];
                }
            }
        }

        glm::vec2 worldOffset = -m_Pivot * 0.5f * worldScale + m_Offset;

        Set( worldAnchor + worldOffset, 0.0f, worldScale );

        for ( UiElement* child : m_Children )
        {
            child->updateTransform();
        }
    }


    /// @brief  callback for whenever the window resizes
    void UiElement::onWindowResizedCallback( glm::ivec2 const& )
    {
        updateTransform();
    }


//-----------------------------------------------------------------------------
// private: reading
//-----------------------------------------------------------------------------
    
    /// @brief  reads the Anchor
    /// @param  data    the json data
    void UiElement::readAnchor( nlohmann::ordered_json const& data )
    {
        Stream::Read( &m_Anchor, data );
    }

    /// @brief  reads the Pivot
    /// @param  data    the json data
    void UiElement::readPivot( nlohmann::ordered_json const& data )
    {
        Stream::Read( &m_Pivot, data );
    }

    /// @brief  reads the Offset
    /// @param  data    the json data
    void UiElement::readOffset( nlohmann::ordered_json const& data )
    {
        Stream::Read( &m_Offset, data );
    }

    /// @brief  reads the FrameSize
    /// @param  data    the json data
    void UiElement::readFrameSize( nlohmann::ordered_json const& data )
    {
        Stream::Read( &m_FrameSize, data );
    }

    /// @brief  reads the ParentName
    /// @param  data    the json data
    void UiElement::readParentName( nlohmann::ordered_json const& data )
    {
        Stream::Read( m_ParentName, data );
    }

    /// @brief  reads the SizeTypes
    /// @param  data    the json data
    void UiElement::readSizeTypes( nlohmann::ordered_json const& data )
    {
        if ( data.is_array() == false )
        {
            Debug()
                << "JSON Warning: unexpected type \"" << data.type_name()
                << "\" encountered, expected type \"Array\" while reading UiElement SizeTypes"
                << std::endl;
            return;
        }

        if ( data.size() != 2 )
        {
            Debug()
                << "JSON Warning: unexpected array size \"" << data.size()
                << "\" encountered, expected size \"2\" while reading UiElement SizeTypes"
                << std::endl;
            return;
        }

        for ( int i = 0; i < 2; ++i )
        {
            if ( data[ i ].is_string() == false )
            {
                Debug()
                    << "JSON Warning: unexpected type \"" << data[ i ].type_name()
                    << "\" encountered, expected type \"String\" while reading UiElement SizeTypes"
                    << std::endl;
                return;
            }

            auto const& sizeType = s_SizeTypes.find( data[ i ] );
            if ( sizeType == s_SizeTypes.end() )
            {
                Debug()
                    << "JSON Warning: unrecognized token \"" << data[ i ]
                    << "\" encountered while reading UiElement SizeTypes"
                    << std::endl;
                return;
            }

            m_SizeTypes[ i ] = sizeType->second;
        }
    }

    /// @brief  map converting strings to size types
    std::map< std::string, UiElement::SizeType > const UiElement::s_SizeTypes = {
        { "Absolute"        , SizeType::Absolute         },
        { "RelativeToWidth" , SizeType::RelativeToWidth  },
        { "RelativeToHeight", SizeType::RelativeToHeight }
    };


    /// @brief  map of read methods
    ReadMethodMap< UiElement > const UiElement::s_ReadMethods = {
        { "Anchor"    , &readAnchor     },
        { "Pivot"     , &readPivot      },
        { "Offset"    , &readOffset     },
        { "FrameSize" , &readFrameSize  },
        { "SizeTypes" , &readSizeTypes  },
        { "ParentName", &readParentName }
    };


//-----------------------------------------------------------------------------
// public: writing
//-----------------------------------------------------------------------------


    /// @brief  writes this UiElement to json
    /// @return the written json data
    nlohmann::ordered_json UiElement::Write() const
    {
        nlohmann::ordered_json json;

        json[ "Anchor" ] = Stream::Write( m_Anchor );
        json[ "Pivot" ] = Stream::Write( m_Pivot );
        json[ "Offset" ] = Stream::Write( m_Offset );
        json[ "FrameSize" ] = Stream::Write( m_FrameSize );

        nlohmann::ordered_json& sizeTypes = json[ "SizeTypes"];
        for ( int i = 0; i < 2; ++i )
        {
            for ( auto const& [ name, sizeType ] : s_SizeTypes )
            {
                if ( sizeType == m_SizeTypes[i] )
                {
                    sizeTypes.push_back( name );
                    break;
                }

                Debug() << "JSON Warning: Unable to write UiElement: unrecognized SizeType" << std::endl;
            }
        }

        if ( m_Parent != nullptr )
        {
            json[ "ParentName" ] = Stream::Write( m_ParentName );
        }

        return json;
    }


//-----------------------------------------------------------------------------
// private: copying
//-----------------------------------------------------------------------------


    /// @brief  copy constructor
    /// @param  other   the other UiElement to copy
    UiElement::UiElement( UiElement const& other ) :
        Transform( other ),
        m_Anchor( other.m_Anchor ),
        m_Pivot( other.m_Pivot ),
        m_Offset( other.m_Offset ),
        m_FrameSize( other.m_FrameSize ),
        m_SizeTypes( other.m_SizeTypes ),
        m_ParentName( other.m_ParentName )
    {}


//-----------------------------------------------------------------------------
