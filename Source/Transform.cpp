///--------------------------------------------------------------------------//
/// @file   Transform.cpp
/// @brief  Function definitions for the Transform class
/// 
/// @author Tyler Birdsall (tyler.birdsall)
/// @date   September 2023
///
/// @copyright (c) 2023 DigiPen (USA) Corporation.
///--------------------------------------------------------------------------//


#include "pch.h" // precompiled header has to be included first
#include "Transform.h"

#include "DebugSystem.h"
#include "RenderSystem.h"
#include "Texture.h"
#include "InputSystem.h"

//-----------------------------------------------------------------------------
// public: constructor
//-----------------------------------------------------------------------------


    /// @brief Default constructor for the transform component.
    Transform::Transform() :
        Component( typeid( Transform ) )
    {}


//-----------------------------------------------------------------------------
// protected: constructor
//-----------------------------------------------------------------------------


    /// @brief  inherited constructor
    /// @param  type    the type of the derived class
    Transform::Transform( std::type_index const& type ) :
        Component( type )
    {}

//-----------------------------------------------------------------------------
// public: methods
//-----------------------------------------------------------------------------

    /// @brief  adds an OnTransformChanged callback to this Transform
    /// @param  callback    the callback function to add
    /// @param  ownerId     the ID of the owner of the callback
    void Transform::AddOnTransformChangedCallback( unsigned ownerId, std::function< void() > callback )
    {
        m_OnTransformChangedCallbacks.emplace( ownerId, std::move( callback ) );
    }

    /// @brief  removes an OnTransformChanged callback from this Transform
    /// @param  ownerId handle to the callback to remove
    void Transform::RemoveOnTransformChangedCallback( unsigned ownerId )
    {
        m_OnTransformChangedCallbacks.erase( ownerId );
    }

//-----------------------------------------------------------------------------
// public: accessors
//-----------------------------------------------------------------------------

    /// @brief  unified set method that sets all three components of Transform at once
    /// @param  translation the new translation to set
    /// @param  rotation    the new rotation to set
    /// @param  scale       the new scale to set
    void Transform::Set( glm::vec2 const& translation, float rotation, glm::vec2 const& scale )
    {
        m_Translation = translation;
        m_Rotation = rotation;
        m_Scale = scale;
        MarkChanged();
    }

    /// @brief  unified set method that sets translation and rotation at once
    /// @param  translation the new translation to set
    /// @param  rotation    the new rotation to set
    void Transform::Set( glm::vec2 const& translation, float rotation )
    {
        m_Translation = translation;
        m_Rotation = rotation;
        MarkChanged();
    }


    /// @brief  gets the translation
    /// @return (gl::vec3)  the translation
    glm::vec2 const& Transform::GetTranslation() const
    {
        return m_Translation;
    }

    /// @brief  sets the translation
    /// @param  translation the new translation
    void Transform::SetTranslation( glm::vec2 const& translation )
    {
        m_Translation = translation;
        MarkChanged();
    }


    /// @brief  gets the rotation
    /// @return (float) the rotation
    float Transform::GetRotation() const
    {
        return m_Rotation;
    }

    /// @brief  sets the rotation
    /// @param  rotation    the new rotation
    void Transform::SetRotation( float rotation )
    {
        m_Rotation = rotation;
        MarkChanged();
    }


    /// @brief  gets the scale
    /// @return (glm::vec3) the scale
    glm::vec2 const& Transform::GetScale() const
    {
        return m_Scale;
    }

    /// @brief  sets the scale
    /// @param  scale   the new scale
    void Transform::SetScale( glm::vec2 const& scale )
    {
        m_Scale = scale;
        MarkChanged();
    }


    /// @brief  gets the isDiagetic flag
    /// @return (bool)  the isDiagetic flag
    bool const& Transform::GetIsDiegetic() const
    {
        return m_IsDiegetic;
    }

    /// @brief  sets the isDiagetic flag
    /// @param  isDiagetic  the new isDiagetic flag
    void Transform::SetIsDiegetic( bool isDiagetic )
    {
        m_IsDiegetic = isDiagetic;
    }


    /// @brief  sets the dirty flag of this Transform
    /// @param  dirty   whether this transform is dirty
    void Transform::SetIsDirty( bool dirty )
    {
        m_IsDirty = true;
    }

    /// @brief  Calculates and gets the translation matrix of the transform component.
    /// @return The translation matrix of the transform component.
    glm::mat4 const& Transform::GetMatrix() const
    {
        if (m_IsDirty)
        {
            glm::mat4 rotate(1);
            glm::mat4 scale(1);
            glm::mat4 translate(1);

            rotate = glm::rotate( rotate, m_Rotation, {0, 0, 1} );
            scale = glm::scale( scale, { m_Scale.x, m_Scale.y, 1.0f } );
            translate = glm::translate( translate, { m_Translation.x, m_Translation.y, 0.0 } );

            // const cast to allow this method to be called on a Transform marked const
            Transform* self = const_cast< Transform* >(this);
            self->m_Matrix = translate * rotate * scale;

            self->m_IsDirty = false;
        }
        return m_Matrix;
    }

    /// @brief			 Sets the transform component's translation matrix.
    /// @param newMatrix The new translation matrix.
    void Transform::SetMatrix(glm::mat4 const& newMatrix)
    {
        m_Matrix = newMatrix;
    }


    /// @brief  gets the Transform debug widget texture
    /// @return the Transform debug widget texture
    Texture const* Transform::GetWidgetTexture()
    {
        if ( s_WidgetTexture == nullptr )
        {
            s_WidgetTexture = new Texture( "Data/Textures/Debug/Transform.png", glm::ivec2( 1 ), glm::vec2( 0.5f / 8.0f ) );
        }
        return s_WidgetTexture;
    }


//-----------------------------------------------------------------------------
// private: virtual override methods
//-----------------------------------------------------------------------------

    void Transform::Inspector()
    {
        // Edit Translation
        glm::vec2 translation = GetTranslation();
        if ( ImGui::DragFloat2( "Translation", &translation[0], 0.05f ) )
        {
            SetTranslation(translation);
        }

        // Edit Rotation
        float rotation = GetRotation();
        if ( ImGui::DragFloat( "Rotation", &rotation, 0.05f ) )
        {
            SetRotation(rotation);
        }

        // Edit Scale
        glm::vec2 scale = GetScale();
        if ( ImGui::DragFloat2( "Scale", &scale[0], 0.05f ) )
        {
            SetScale(scale);
        }

        // Toggle Diegetic
        bool isDiegetic = GetIsDiegetic();
        if (ImGui::Checkbox("Is Diegetic", &isDiegetic))
        {
            SetIsDiegetic(isDiegetic);
        }

        DrawDebugWidget();
        DebugDrag();
    }

//-----------------------------------------------------------------------------
// private: static variables
//-----------------------------------------------------------------------------


    /// @brief  texture of the Transform debug widget
    Texture const* Transform::s_WidgetTexture = nullptr;


//-----------------------------------------------------------------------------
// private: methods
//-----------------------------------------------------------------------------

    /// @brief  marks this Transform as dirty and calls the OnTransformChanged callbacks
    void Transform::MarkChanged()
    {
        m_IsDirty = true;

        for ( auto callback : m_OnTransformChangedCallbacks )
        {
            callback.second();
        }
    }

    /// @brief  draws this Transform's debug widget
    void Transform::DrawDebugWidget() const
    {
        Renderer()->DrawTexture( GetWidgetTexture(), m_Translation, glm::vec2( 1 ), m_Rotation, glm::vec4( 0 ), 1.0f, m_IsDiegetic );
    }

    /// @brief  allows dragging of Transforms with the mouse
    void Transform::DebugDrag()
    {
        float constexpr maxDragRange = 1.0f;

        static unsigned draggedTransformId = 0;
        static float closestDistanceSquared = maxDragRange * maxDragRange;

        if ( Input()->GetMouseTriggered( GLFW_MOUSE_BUTTON_1 ) )
        {
            glm::vec2 mousePos = m_IsDiegetic ? Input()->GetMousePosWorld() : Input()->GetMousePosUI();
            glm::vec2 offset = m_Translation - mousePos;
            float distanceSquared = glm::dot( offset, offset );
            if ( distanceSquared <= closestDistanceSquared )
            {
                closestDistanceSquared = distanceSquared;
                draggedTransformId = GetId();
            }
        }
        else if (
            draggedTransformId == GetId() &&
            Input()->GetMouseDown( GLFW_MOUSE_BUTTON_1 )
        )
        {
            glm::vec2 mousePos = m_IsDiegetic ? Input()->GetMousePosWorld() : Input()->GetMousePosUI();
            SetTranslation( mousePos );
        }
        else if ( Input()->GetMouseReleased( GLFW_MOUSE_BUTTON_1 ) )
        {
            draggedTransformId = 0;
            closestDistanceSquared = maxDragRange * maxDragRange;
        }
    }

//-----------------------------------------------------------------------------
// private: reading
//-----------------------------------------------------------------------------


    /// @brief			 Reads in a translation vector from a JSON value.
    /// @param jsonValue The value to read from.
    void Transform::readTranslation( nlohmann::ordered_json const& data )
    {
	    m_Translation = Stream::Read< 2, float >(data);
    }

    /// @brief			 Reads in a rotation from a JSON value.
    /// @param jsonValue The value to read from.
    void Transform::readRotation( nlohmann::ordered_json const& data )
    {
	    m_Rotation = Stream::Read<float>(data);
    }

    /// @brief			 Reads in a scale vector from a JSON value.
    /// @param jsonValue The value to read from.
    void Transform::readScale( nlohmann::ordered_json const& data )
    {
	    m_Scale = Stream::Read< 2, float >(data);
    }

    /// @brief			 Reads in a flag from a JSON value.
    /// @param jsonValue The value to read from.
    void Transform::readIsDiegetic( nlohmann::ordered_json const& data )
    {
	    m_IsDiegetic = Stream::Read<bool>(data);
    }


    /// @brief  called after finished reading
    void Transform::AfterLoad()
    {
        MarkChanged();
    }

    /// @brief Write all Transform component data to a JSON file.
    /// @return The JSON file containing the Transform component data.
    nlohmann::ordered_json Transform::Write() const
    {
        nlohmann::ordered_json data;

        data["Translation"] = Stream::Write(m_Translation);
        data["Rotation"] = m_Rotation;
        data["Scale"] = Stream::Write(m_Scale);
        data["IsDiegetic"] = m_IsDiegetic;

        return data;
    }

    // Map of all the read methods for the transform component.
    ReadMethodMap< Transform > Transform::s_ReadMethods = {
	    { "Translation" , &readTranslation },
	    { "Rotation"	, &readRotation    },
	    { "Scale"		, &readScale       },
	    { "IsDiegetic"	, &readIsDiegetic  }
    };


//-----------------------------------------------------------------------------
// protected: copying
//-----------------------------------------------------------------------------


    /// @brief		 Copy constructor for the transform component.
    /// @param other Reference to another transform component.
    Transform::Transform(const Transform& other) :
        Component( other ),
        m_Translation( other.m_Translation ),
        m_Rotation(    other.m_Rotation    ),
        m_Scale(       other.m_Scale       ),
        m_IsDirty(     other.m_IsDirty     ),
        m_Matrix(      other.m_Matrix      ),
        m_IsDiegetic(  other.m_IsDiegetic  ),
        m_OnTransformChangedCallbacks()
    {}


//-----------------------------------------------------------------------------
