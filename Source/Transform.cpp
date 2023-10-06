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

//-----------------------------------------------------------------------------
// public: constructor / destructor
//-----------------------------------------------------------------------------


    /// @brief Default constructor for the transform component.
    Transform::Transform() :
        Component( typeid( Transform ) ),
	    m_Translation( 0, 0, 1 ),
	    m_Rotation( 0.0f ),
	    m_Scale( 1, 1, 0 ),
	    m_IsDirty( true ),
	    m_Matrix( glm::mat4(0.0f) ),
	    m_IsDiegetic( true ),
        m_Parent( nullptr )
    {}


//-----------------------------------------------------------------------------
// public: accessors
//-----------------------------------------------------------------------------


    /// @brief  Calculates and gets the translation matrix of the transform component.
    /// @return The translation matrix of the transform component.
    glm::mat4 const& Transform::GetMatrix() const
    {
        if (m_IsDirty)
        {
            glm::mat4 rotateTemp(1);
            glm::mat4 scaleTemp(1);
            glm::mat4 transferTemp(1);
            glm::mat4 result;

            rotateTemp = glm::rotate(rotateTemp, m_Rotation, {0, 0, 1});
            scaleTemp = glm::scale(scaleTemp, m_Scale);
            transferTemp = glm::translate(transferTemp, m_Translation);
            result = rotateTemp * scaleTemp;

            Transform* self = const_cast< Transform* >(this);
            self->m_Matrix = transferTemp * result;

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
// private: reading
//-----------------------------------------------------------------------------


    /// @brief			 Reads in a translation vector from a JSON value.
    /// @param jsonValue The value to read from.
    void Transform::readTranslation( Stream jsonValue )
    {
	    m_Translation = jsonValue.Read<glm::vec3>();
	    m_IsDirty = true;
    }

    /// @brief			 Reads in a rotation from a JSON value.
    /// @param jsonValue The value to read from.
    void Transform::readRotation( Stream jsonValue )
    {
	    m_Rotation = jsonValue.Read<float>();
	    m_IsDirty = true;
    }

    /// @brief			 Reads in a scale vector from a JSON value.
    /// @param jsonValue The value to read from.
    void Transform::readScale( Stream jsonValue )
    {
	    m_Scale = jsonValue.Read<glm::vec3>();
	    m_IsDirty = true;
    }

    /// @brief			 Reads in a flag from a JSON value.
    /// @param jsonValue The value to read from.
    void Transform::readIsDiegetic( Stream jsonValue )
    {
	    m_IsDiegetic = jsonValue.Read<bool>();
    }


    // Map of all the read methods for the transform component.
    ReadMethodMap< Transform > Transform::s_ReadMethods = {
	    { "translation" , &readTranslation },
	    { "rotation"	, &readRotation    },
	    { "scale"		, &readScale       },
	    { "diegetic"	, &readIsDiegetic	   }
    };

    /// @brief  Gets a map of the read methods for transform component.
    /// @return A map of all the transform component read methods.
    ReadMethodMap< Component > const& Transform::GetReadMethods() const
    {
	    return (ReadMethodMap< Component > const&)s_ReadMethods;
    }


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
        m_Parent(      other.m_Parent      )
    {}


//-----------------------------------------------------------------------------
