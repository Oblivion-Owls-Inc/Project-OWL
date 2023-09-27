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
#include <rapidjson/document.h> // rapidjson::Value

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
	void Read( const rapidjson::Value& data );

	vec3* GetTranslation();
	const vec3* GetTranslation() const;
	void SetTranslation(vec3 newTranslation);

	float GetRotation() const;
	void SetRotation(float newRotation);

	vec3* GetScale();
	const vec3* GetScale() const;
	void SetScale(vec3 newScale);

	glm::mat4* GetMatrix();

	void SetMatrix(glm::mat4 newMatrix);

	bool GetIsDirty() const;
	void SetIsDirty(bool newIsDirty);

	bool getIsDiegetic() const;
	void setIsDiegetic(bool newIsDiegetic);

private:

	void ReadTranslation( Stream jsonValue );
	void ReadRotation( Stream jsonValue );
	void ReadScale( Stream jsonValue );
	void ReadDiegetic( Stream jsonValue );

	static std::map< std::string, ReadMethod< Transform > > s_ReadMethods;

	virtual std::map< std::string, ReadMethod< Component > > const& GetReadMethods();
	

protected:

private:
    vec3 translation;
    vec3 scale;
    float rotation;
    glm::mat4 matrix;
    bool isDirty;
	bool isDiegetic;
};

