///--------------------------------------------------------------------------//
/// @file   Transform.cpp
/// @brief  Function definitions for the Transform class
/// 
/// @author Tyler Birdsall (tyler.birdsall)
/// @date   September 2023
///
/// @copyright (c) 2023 DigiPen (USA) Corporation.
///--------------------------------------------------------------------------//

#include "Transform.h"

#include "DebugSystem.h"
#include "RenderSystem.h"
#include "Texture.h"
#include "InputSystem.h"

//-----------------------------------------------------------------------------
// public: constructor / destructor
//-----------------------------------------------------------------------------


    /// @brief Default constructor for the transform component.
    Transform::Transform() :
        Component( typeid( Transform ) )
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
        static Texture const texture = Texture( "Data/Textures/DebugTransform.png", glm::ivec2( 1 ), glm::vec2( 0.0625f ) );
        Renderer()->DrawTexture( &texture, m_Translation, glm::vec2( 1 ), m_Rotation, glm::vec4( 0 ), 1.0f, m_IsDiegetic );
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
// private: copying
//-----------------------------------------------------------------------------

    /// @brief  Clones a transform component.
    /// @return A new transform component.
    Component* Transform::Clone() const
    {
        return (Component*)new Transform( *this );
    }


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
        m_Parent(      other.m_Parent      ),
        m_OnTransformChangedCallbacks()
    {}


//-----------------------------------------------------------------------------
