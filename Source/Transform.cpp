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

glm::vec3* Transform::getTranslation()
{
	return &translation;
}

const glm::vec3* Transform::getTranslation() const
{
	return &translation;
}

void Transform::setTranslation(glm::vec3 newTranslation)
{
	translation = newTranslation;
	setIsDirty(true);
}

float Transform::getRotation() const
{
	return 0.0f;
}

void Transform::setRotation(float newRotation)
{
	rotation = newRotation;
	setIsDirty(true);
}

glm::vec3* Transform::getScale()
{
	return &scale;
}

const glm::vec3* Transform::getScale() const
{
	return &scale;
}

void Transform::setScale(glm::vec3 newScale)
{
	scale = newScale;
	setIsDirty(true);
}

glm::mat4* Transform::getMatrix()
{
	if (isDirty)
	{
		glm::mat4 rotateTemp(1);
		glm::mat4 scaleTemp(1);
		glm::mat4 transferTemp(1);
		glm::mat4 result;

		//glm::mat4(glm::mat2(4, 0, 0, 4));
		

		
		rotateTemp = glm::rotate(rotateTemp, rotation, {0, 0, 1});
		scaleTemp = glm::scale(scaleTemp, scale);
		transferTemp = glm::translate(transferTemp, translation);
		result = rotateTemp * scaleTemp;
		matrix = transferTemp * result;
		
		isDirty = false;
	}
	return &matrix;
}


void Transform::setMatrix(glm::mat4 newMatrix)
{
	matrix = newMatrix;
}

bool Transform::getIsDirty() const
{
	return isDirty;
}

void Transform::setIsDirty(bool newIsDirty)
{
	isDirty = newIsDirty;
}



