/*****************************************************************//**
 * \file   Transform.cpp
 * \brief  Contains functions for transformations
 * 
 * \author Tyler Birdsall (tyler.birdsall@digipen.edu)
 * \date   September 2023
 *********************************************************************/

#include "Transform.h"

Transform::Transform() : Component(typeid(Transform))
{
	translation = { 0, 0, 0 };
	rotation = 0.0f;
	scale = { 1, 1, 0 };
	isDirty = true;
	matrix = glm::mat4(0.0f);
}

Transform::Transform(const Transform& other) : Component(other)
{
	translation = other.translation;
	rotation = other.rotation;
	scale = other.scale;
	isDirty = other.isDirty;
	matrix = other.matrix;	
}

Transform::~Transform(void)
{

}

/**
 * \brief Clones a transform
 * 
 * \return 
 */
Component* Transform::Clone() const
{
	return (Component*) new Transform(*this);
}


glm::vec3* Transform::GetTranslation()
{
	return &translation;
}

const glm::vec3* Transform::GetTranslation() const
{
	return &translation;
}

void Transform::SetTranslation(glm::vec3 newTranslation)
{
	translation = newTranslation;
	SetIsDirty(true);
}

float Transform::GetRotation() const
{
	return 0.0f;
}

void Transform::SetRotation(float newRotation)
{
	rotation = newRotation;
	SetIsDirty(true);
}

glm::vec3* Transform::GetScale()
{
	return &scale;
}

const glm::vec3* Transform::GetScale() const
{
	return &scale;
}

void Transform::SetScale(glm::vec3 newScale)
{
	scale = newScale;
	SetIsDirty(true);
}

glm::mat4* Transform::GetMatrix()
{
	if (isDirty)
	{
		glm::mat4 rotateTemp(1);
		glm::mat4 scaleTemp(1);
		glm::mat4 transferTemp(1);
		glm::mat4 result;

		rotateTemp = glm::rotate(rotateTemp, rotation, {0, 0, 1});
		scaleTemp = glm::scale(scaleTemp, scale);
		transferTemp = glm::translate(transferTemp, translation);
		result = rotateTemp * scaleTemp;
		matrix = transferTemp * result;
		
		isDirty = false;
	}
	return &matrix;
}


void Transform::SetMatrix(glm::mat4 newMatrix)
{
	matrix = newMatrix;
}

bool Transform::GetIsDirty() const
{
	return isDirty;
}

void Transform::SetIsDirty(bool newIsDirty)
{
	isDirty = newIsDirty;
}


void Transform::ReadTranslation( Stream jsonValue )
{
	translation = jsonValue.Read<glm::vec3>();
	isDirty = true;
}
void Transform::ReadRotation( Stream jsonValue )
{
	rotation = jsonValue.Read<float>();
	isDirty = true;
}
void Transform::ReadScale( Stream jsonValue )
{
	scale = jsonValue.Read<glm::vec3>();
	isDirty = true;
}

std::map< std::string, ReadMethod< Transform > > Transform::readMethods = {
	{ "translation", &ReadTranslation },
	{ "rotation", &ReadRotation },
	{ "scale", &ReadScale }
};

std::map< std::string, ReadMethod< Component > > const& Transform::GetReadMethods()
{
	return (std::map< std::string, ReadMethod< Component > > const&)readMethods;
}