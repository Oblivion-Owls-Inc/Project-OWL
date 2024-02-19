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
#include "Entity.h"
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
// public: accessors
//-----------------------------------------------------------------------------


    /// @brief  gets the parent UiElement
    /// @return the parent UiElement
    UiElement* UiElement::GetParentElement() const
    {
        return m_ParentElement;
    }


    /// @brief  gets the anchor position of this UiElement
    /// @return the anchor position
    glm::vec2 const& UiElement::GetAnchor() const
    {
        return m_Anchor;
    }

    /// @brief  sets the anchor position of this UiElement
    /// @param  anchor  the anchor position
    void UiElement::SetAnchor( glm::vec2 const& anchor )
    {
        m_Anchor = anchor;
    }


    /// @brief  gets the pivot position of this UiElement
    /// @return the pivot position
    glm::vec2 const& UiElement::GetPivot() const
    {
        return m_Pivot;
    }

    /// @brief  sets the pivot position of this UiElement
    /// @param  pivot  the pivot position
    void UiElement::SetPivot( glm::vec2 const& pivot )
    {
        m_Pivot = pivot;
    }


    /// @brief  gets the offset of this UiElement
    /// @return the offset
    glm::vec2 const& UiElement::GetOffset() const
    {
        return m_Offset;
    }

    /// @brief  sets the offset of this UiElement
    /// @param  offset  the offset
    void UiElement::SetOffset( glm::vec2 const& offset )
    {
        m_Offset = offset;
    }


    /// @brief  gets the frame size of this UiElement
    /// @return the frame size
    glm::vec2 const& UiElement::GetFrameSize() const
    {
        return m_FrameSize;
    }

    /// @brief  sets the frame size of this UiElement
    /// @param  frameSize   the frame size
    void UiElement::SetFrameSize( glm::vec2 const& frameSize )
    {
        m_FrameSize = frameSize;
    }


    /// @brief  gets the size types of this UiElement
    /// @return the size types
    UiElement::SizeTypeVec const& UiElement::GetSizeTypes() const
    {
        return m_SizeTypes;
    }

    /// @brief  sets the size types of this UiElement
    /// @param  sizeTypes   the size types
    void UiElement::SetSizeTypes( SizeTypeVec const& sizeTypes )
    {
        m_SizeTypes = sizeTypes;
    }


//-----------------------------------------------------------------------------
// public: virtual override methods
//-----------------------------------------------------------------------------

    
    /// @brief  called once when entering the scene
    void UiElement::OnInit()
    {
        Platform()->AddOnWindowResizeCallback( GetId(), std::bind( &UiElement::onWindowResizedCallback, this, std::placeholders::_1 ) );

        Entity* parent = GetEntity()->GetParent();
        m_ParentElement = parent == nullptr ? nullptr : parent->GetComponent< UiElement >();
        updateTransform();
    }

    /// @brief  called once when leaving the scene
    void UiElement::OnExit()
    {
        Platform()->RemoveOnWindowResizeCallback( GetId() );
    }


    /// @brief  called every time after the Entity this Component is attached to's heirarchy changes
    void UiElement::OnHierarchyChange( Entity* previousParent )
    {
        Entity* parent = GetEntity()->GetParent();
        m_ParentElement = parent == nullptr ? nullptr : parent->GetComponent< UiElement >();
        updateTransform();
    }


//-----------------------------------------------------------------------------
// private: methods
//-----------------------------------------------------------------------------


    /// @brief  updates the Transform and all children transforms
    void UiElement::updateTransform()
    {

        glm::vec2 parentScale;
        if ( m_ParentElement == nullptr )
        {
            glm::mat4 clipToUi = glm::inverse( Cameras()->GetMat_UItoClip() );
            parentScale = clipToUi * glm::vec4( 1, 1, 0, 0 ) * 2.0f;
        }
        else
        {
            parentScale = m_ParentElement->GetScale();
        }


        glm::vec2 worldAnchor = (m_ParentElement ? m_ParentElement->GetTranslation() : glm::vec2( 0 )) + 0.5f * m_Anchor * parentScale;

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
    }


    /// @brief  callback for whenever the window resizes
    void UiElement::onWindowResizedCallback( glm::ivec2 const& )
    {
        updateTransform();
    }


//-----------------------------------------------------------------------------
// public: inspection
//-----------------------------------------------------------------------------


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

    /// @brief  reads the SizeTypes
    /// @param  data    the json data
    void UiElement::readSizeTypes( nlohmann::ordered_json const& data )
    {
        if ( data.is_array() == false )
        {
            Debug()  << "Warning:"
                << "JSON: unexpected type \"" << data.type_name()
                << "\" encountered, expected type \"Array\" while reading UiElement SizeTypes"
                << std::endl;
            return;
        }

        if ( data.size() != 2 )
        {
            Debug() << "Warning:"
                << "JSON Warning: unexpected array size \"" << data.size()
                << "\" encountered, expected size \"2\" while reading UiElement SizeTypes"
                << std::endl;
            return;
        }

        for ( int i = 0; i < 2; ++i )
        {
            if ( data[ i ].is_string() == false )
            {
                Debug() << "Warning:"
                    << "JSON Warning: unexpected type \"" << data[ i ].type_name()
                    << "\" encountered, expected type \"String\" while reading UiElement SizeTypes"
                    << std::endl;
                return;
            }

            auto const& sizeType = s_SizeTypes.find( data[ i ] );
            if ( sizeType == s_SizeTypes.end() )
            {
                Debug() << "Warning:"
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


//-----------------------------------------------------------------------------
// public: reading / writing
//-----------------------------------------------------------------------------


    /// @brief  gets the map of read methods
    /// @return the map of read methods
    ReadMethodMap< ISerializable > const& UiElement::GetReadMethods() const
    {
        static ReadMethodMap< UiElement > const readMethods = {
            { "Anchor"    , &UiElement::readAnchor     },
            { "Pivot"     , &UiElement::readPivot      },
            { "Offset"    , &UiElement::readOffset     },
            { "FrameSize" , &UiElement::readFrameSize  },
            { "SizeTypes" , &UiElement::readSizeTypes  }
        };

        return (ReadMethodMap< ISerializable > const&)readMethods;
    }


    /// @brief  writes this UiElement to json
    /// @return the written json data
    nlohmann::ordered_json UiElement::Write() const
    {
        nlohmann::ordered_json json;

        json[ "Anchor"    ] = Stream::Write( m_Anchor    );
        json[ "Pivot"     ] = Stream::Write( m_Pivot     );
        json[ "Offset"    ] = Stream::Write( m_Offset    );
        json[ "FrameSize" ] = Stream::Write( m_FrameSize );

        nlohmann::ordered_json& sizeTypes = json[ "SizeTypes" ];
        for ( int i = 0; i < 2; ++i )
        {
            bool found = false;
            for ( auto const& [ name, sizeType ] : s_SizeTypes )
            {
                if ( sizeType == m_SizeTypes[ i ] )
                {
                    sizeTypes.push_back( name );
                    found = true;
                    break;
                }
            }

            if ( found == false )
            {
                Debug() << "WARNING: Unable to write UiElement: unrecognized SizeType" << std::endl;
                return json;
            }
        }

        return json;
    }

    
//-----------------------------------------------------------------------------
// public: copying
//-----------------------------------------------------------------------------


    /// @brief  creates a new copy of this UiElement
    /// @return the newly created UiElement
    UiElement* UiElement::Clone() const
    {
        return new UiElement( *this );
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
        m_SizeTypes( other.m_SizeTypes )
    {}


//-----------------------------------------------------------------------------
