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

class Transform : public Component
{
//-----------------------------------------------------------------------------
public: // constructor / destructor / inspector
//-----------------------------------------------------------------------------

    /// @brief  constructor
    Transform();

    /// @brief destructor
    ~Transform() = default;

    /// @brief  inspector for this component
    virtual void Inspector() override;

//-----------------------------------------------------------------------------
public: // accessors
//-----------------------------------------------------------------------------

	/// @brief  gets the translation
	/// @return (gl::vec3)  the translation
	__inline glm::vec3 const& GetTranslation() const { return m_Translation; }

	/// @brief  sets the translation
	/// @param  translation the new translation
	__inline void SetTranslation( glm::vec3 const& translation ) { m_Translation = translation; m_IsDirty = true; }


    /// @brief  gets the rotation
    /// @return (float) the rotation
	__inline float GetRotation() const { return m_Rotation; }

    /// @brief  sets the rotation
    /// @param  rotation    the new rotation
	__inline void SetRotation( float rotation ) { m_Rotation = rotation; m_IsDirty = true; }


    /// @brief  gets the scale
    /// @return (glm::vec3) the scale
    __inline glm::vec3 const& GetScale() const { return m_Scale; }

    /// @brief  sets the scale
    /// @param  scale   the new scale
    __inline void SetScale( glm::vec3 const& scale ) { m_Scale = scale; m_IsDirty = true; }


    /// @brief  gets the isDiagetic flag
    /// @return (bool)  the isDiagetic flag
    __inline bool const& GetIsDiegetic() const { return m_IsDiegetic; }

    /// @brief  sets the isDiagetic flag
    /// @param  isDiagetic  the new isDiagetic flag
    __inline void SetIsDiegetic( bool const& isDiagetic ) { m_IsDiegetic = isDiagetic; }

    /// @brief  marks the transform as dirty;
    __inline void MarkDirty() { m_IsDirty = true; }


    /// @brief  gets the matrix
    /// @return (glm::mat4) the matrix
    glm::mat4 const& GetMatrix() const;

    /// @brief  sets the matrix
    /// @param  matrix  the new matrix
    void SetMatrix( glm::mat4 const& matrix );

//-----------------------------------------------------------------------------
private: // reading
//-----------------------------------------------------------------------------

	
    /// @brief  reads the translation
    /// @param  jsonValue   the json data to read from
    void readTranslation( Stream jsonValue );
	
    /// @brief  reads the rotation
    /// @param  jsonValue   the json data to read from
    void readRotation( Stream jsonValue );

    /// @brief  reads the scale
    /// @param  jsonValue   the json data to read from
    void readScale( Stream jsonValue );
	
    /// @brief  reads the isDiagetic flag
    /// @param  jsonValue   the json data to read from
    void readIsDiegetic( Stream jsonValue );


    /// @brief  map of read methods
	static ReadMethodMap< Transform > s_ReadMethods;

    /// @brief  gets the map of read methods
    /// @return the map of read methods
	virtual ReadMethodMap< Component > const& GetReadMethods() const override;

//-----------------------------------------------------------------------------
private: // copying
//-----------------------------------------------------------------------------

    /// @brief  creates a new copy of this Component
    /// @return the newly created component
    virtual Component* Clone() const override;

    /// @brief  copy constructor
    /// @param  other   the other Transform to copy
    Transform( Transform const& other );

//-----------------------------------------------------------------------------
private: // member variables
//-----------------------------------------------------------------------------

    /// @brief  the position of this Transform
    glm::vec3 m_Translation;

    /// @brief  the scale of this Transform
    glm::vec3 m_Scale;

    /// @brief  the rotation of this Transform
    float m_Rotation;

    /// @brief  the matrix for this Transform
    glm::mat4 m_Matrix;

    /// @brief  flag for when the matrix needs to be regenerated
    bool m_IsDirty;

	/// @brief  whether this Transform exists in world or screen space
	bool m_IsDiegetic;

	/// @brief  the Transform that this Transform descends from
	Transform* m_Parent;

//-----------------------------------------------------------------------------
};
