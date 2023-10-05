/*****************************************************************//**
 * \file   Transform.h
 * \brief  Transform
 * 
 * \author Tyler Birdsall (tyler.birdsall@digipen.edu)
 * \date   September 2023
 *********************************************************************/

#pragma once
#include "Component.h"
#include "basics.h"
#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"
#include <rapidjson/document.h> // rapidjson::Value

using namespace glm;
class Transform : public Component
{
public:
    Transform();
    Transform(const Transform& other);

    ~Transform() = default;

public:
    Component* Clone() const override;

	vec3 const& GetTranslation() const;
	void SetTranslation(vec3 const& newTranslation);

	float GetRotation() const;
	void SetRotation(float newRotation);

	vec3 const& GetScale() const;
	void SetScale(vec3 newScale);

	glm::mat4 const& GetMatrix() const;
	void SetMatrix(glm::mat4 const& newMatrix);

	bool GetIsDirty() const;
	void SetIsDirty(bool newIsDirty);

	bool GetIsDiegetic() const;
	void SetIsDiegetic(bool newIsDiegetic);

private:

	void ReadTranslation( Stream jsonValue );
	void ReadRotation( Stream jsonValue );
	void ReadScale( Stream jsonValue );
	void ReadDiegetic( Stream jsonValue );

	static ReadMethodMap< Transform > s_ReadMethods;

	virtual ReadMethodMap< Component > const& GetReadMethods() const override;

protected:

private:
    vec3 m_Translation;
    vec3 m_Scale;
    float m_Rotation;
    glm::mat4 m_Matrix;
    bool m_IsDirty;
	bool m_IsDiegetic;
};

