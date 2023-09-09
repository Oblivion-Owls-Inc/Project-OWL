/*****************************************************************//**
 * \file   Transform.h
 * \brief  Transform
 * 
 * \author thatg
 * \date   September 2023
 *********************************************************************/

#pragma once
#include "Component.h"
#include "basics.h"
#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"

using namespace glm;
class Transform :
    public Component
{
public:
    Transform();
    Transform(const Transform& other);

    ~Transform(void);

public:
    Component* Clone() const override;

	vec3* getTranslation();
	const vec3* getTranslationConst() const;
	void setTranslation(vec3 newTranslation);

	float getRotation() const;
	void setRotation(float newRotation);

	vec3* getScale();
	const vec3* getScaleConst() const;
	void setScale(vec3 newScale);

	glm::mat4* getMatrix();
	static void matrixRotate(glm::mat4* matrix, float angle);

	void setMatrix(glm::mat4 newMatrix);

	bool getIsDirty() const;
	void setIsDirty(bool newIsDirty);

	

protected:

private:
    vec3 translation;
    vec3 scale;
    float rotation;
    glm::mat4 matrix;
    bool isDirty;
};

