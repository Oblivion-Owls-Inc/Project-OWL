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
#include "document.h" // rapidjson::Value

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

	vec3* getTranslation();
	const vec3* getTranslation() const;
	void setTranslation(vec3 newTranslation);

	float getRotation() const;
	void setRotation(float newRotation);

	vec3* getScale();
	const vec3* getScale() const;
	void setScale(vec3 newScale);

	glm::mat4* getMatrix();

	void setMatrix(glm::mat4 newMatrix);

	bool getIsDirty() const;
	void setIsDirty(bool newIsDirty);

	bool getIsDiegetic() const;
	void setIsDiegetic(bool newIsDiegetic);

private:

	void ReadTranslation( Stream jsonValue );
	void ReadRotation( Stream jsonValue );
	void ReadScale( Stream jsonValue );
	void ReadDiegetic( Stream jsonValue );

	static std::map< std::string, ReadMethod< Transform > > readMethods;

	virtual std::map< std::string, ReadMethod< Component > > const& getReadMethods();
	

protected:

private:
    vec3 translation;
    vec3 scale;
    float rotation;
    glm::mat4 matrix;
    bool isDirty;
	bool isDiegetic;
};

