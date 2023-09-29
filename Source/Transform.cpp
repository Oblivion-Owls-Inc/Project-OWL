///--------------------------------------------------------------------------//
/// @file   Transform.cpp
/// @brief  Function definitions for the Transform class
/// 
/// @author Tyler Birdsall (tyler.birdsall)
/// @date   September 2023
///
/// @copyright © 2023 DigiPen (USA) Corporation.
///--------------------------------------------------------------------------//
#include "Transform.h"

/// @brief Default constructor for the transform component.
Transform::Transform() : Component(typeid(Transform))
{
	m_Translation = { 0, 0, 0 };
	m_Rotation = 0.0f;
	m_Scale = { 1, 1, 0 };
	m_IsDirty = true;
	m_Matrix = glm::mat4(0.0f);
	m_IsDiegetic = false;
}

/// @brief		 Copy constructor for the transform component.
/// @param other Reference to another transform component.
Transform::Transform(const Transform& other) : Component(other)
{
	m_Translation = other.m_Translation;
	m_Rotation = other.m_Rotation;
	m_Scale = other.m_Scale;
	m_IsDirty = other.m_IsDirty;
	m_Matrix = other.m_Matrix;	
	m_IsDiegetic = other.m_IsDiegetic;
}

Transform::~Transform(void)
{

}

/// @brief  Clones a transform component.
/// @return A new transform component.
Component* Transform::Clone() const
{
	return (Component*) new Transform(*this);
}

/// @brief  Gets the translation vector of the transform component.
/// @return The translation vector of the transform component.
glm::vec3* Transform::GetTranslation()
{
	return &m_Translation;
}

/// @brief  Gets a constant version of the translation vector.
/// @return Constant version of the translation vector.
const glm::vec3* Transform::GetTranslation() const
{
	return &m_Translation;
}

/// @brief                Sets the value of the translation vector.
/// @param newTranslation The new translation vector. 
void Transform::SetTranslation(glm::vec3 newTranslation)
{
	m_Translation = newTranslation;
	m_IsDirty = true;
}

/// @brief  Gets the rotation value of the transform component.
/// @return The rotation value of the transform component.
float Transform::GetRotation() const
{
	return 0.0f;
}

/// @brief			   Sets the rotation value of the transform component.
/// @param newRotation The new value of the transform's rotation.
void Transform::SetRotation(float newRotation)
{
	m_Rotation = newRotation;
	m_IsDirty = true;
}

/// @brief  Gets the scale vector of the transform component.
/// @return The scale vector of the transform component.
glm::vec3* Transform::GetScale()
{
	return &m_Scale;
}

/// @brief  Gets a constant version of the transform's scale vector.
/// @return A constant version of the transform's scale vector.
const glm::vec3* Transform::GetScale() const
{
	return &m_Scale;
}

/// @brief		    Sets the scale vector of the transform component.
/// @param newScale The new value of the scale vector
void Transform::SetScale(glm::vec3 newScale)
{
	m_Scale = newScale;
	m_IsDirty = true;
}

/// @brief  Calculates and gets the translation matrix of the transform component.
/// @return The translation matrix of the transform component.
glm::mat4* Transform::GetMatrix()
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
		m_Matrix = transferTemp * result;
		
		m_IsDirty = false;
	}
	return &m_Matrix;
}

/// @brief			 Sets the transform component's translation matrix.
/// @param newMatrix The new translation matrix.
void Transform::SetMatrix(glm::mat4 newMatrix)
{
	m_Matrix = newMatrix;
}

/// @brief  Gets the status of the is dirty flag.
/// @return Whether or not the transform component is dirty.
bool Transform::GetIsDirty() const
{
	return m_IsDirty;
}

/// @brief			  Sets the is dirty flag on the transform component.
/// @param newIsDirty New value for the is dirty flag.
void Transform::SetIsDirty(bool newIsDirty)
{
	m_IsDirty = newIsDirty;
}

bool Transform::GetIsDiegetic() const
{
	return m_IsDiegetic;
}

void Transform::SetIsDiegetic(bool newIsDiegetic)
{
	m_IsDiegetic = newIsDiegetic;
}

//-----------------------------------------------------------------------------
// private: reading
//-----------------------------------------------------------------------------

/// @brief			 Reads in a translation vector from a JSON value.
/// @param jsonValue The value to read from.
void Transform::ReadTranslation( Stream jsonValue )
{
	m_Translation = jsonValue.Read<glm::vec3>();
	m_IsDirty = true;
}

/// @brief			 Reads in a rotation from a JSON value.
/// @param jsonValue The value to read from.
void Transform::ReadRotation( Stream jsonValue )
{
	m_Rotation = jsonValue.Read<float>();
	m_IsDirty = true;
}

/// @brief			 Reads in a scale vector from a JSON value.
/// @param jsonValue The value to read from.
void Transform::ReadScale( Stream jsonValue )
{
	m_Scale = jsonValue.Read<glm::vec3>();
	m_IsDirty = true;
}

/// @brief			 Reads in a flag from a JSON value.
/// @param jsonValue The value to read from.
void Transform::ReadDiegetic( Stream jsonValue )
{
	m_IsDiegetic = jsonValue.Read<bool>();
}

// Map of all the read methods for the transform component.
ReadMethodMap< Transform > Transform::s_ReadMethods = {
	{ "translation" , &ReadTranslation },
	{ "rotation"	, &ReadRotation    },
	{ "scale"		, &ReadScale       },
	{ "diegetic"	, &ReadDiegetic	   }
};

/// @brief  Gets a map of the read methods for transform component.
/// @return A map of all the transform component read methods.
ReadMethodMap< Component > const& Transform::GetReadMethods() const
{
	return (ReadMethodMap< Component > const&)s_ReadMethods;
}