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

#include <functional>
#include <map>

class Transform : public Component
{
//-----------------------------------------------------------------------------
public: // constructor / destructor / inspector
//-----------------------------------------------------------------------------

    /// @brief  constructor
    Transform();

    /// @brief destructor
    ~Transform() = default;

//-----------------------------------------------------------------------------
public: // methods
//-----------------------------------------------------------------------------
    
    /// @brief  adds an OnTransformChanged callback to this Transform
    /// @param  callback    the callback function to add
    /// @return a handle to the added callback
    unsigned AddOnTransformChangedCallback( std::function< void() > callback );

    /// @brief  removes an OnTransformChanged callback from this Transform
    /// @param  callbackHandle  handle to the callback to remove
    void RemoveOnTransformChangedCallback( unsigned callbackHandle );



//-----------------------------------------------------------------------------
public: // accessors
//-----------------------------------------------------------------------------

    /// @brief  unified set method that sets all three components of Transform at once
    /// @param  translation the new translation to set
    /// @param  rotation    the new rotation to set
    /// @param  scale       the new scale to set
    void Set( glm::vec2 const& translation, float rotation = 0.0f, glm::vec2 const& scale = { 1.0f, 1.0f } );


	/// @brief  gets the translation
	/// @return (gl::vec3)  the translation
	glm::vec2 const& GetTranslation() const { return m_Translation; }

	/// @brief  sets the translation
	/// @param  translation the new translation
	void SetTranslation( glm::vec2 const& translation ) { m_Translation = translation; MarkChanged(); }


    /// @brief  gets the rotation
    /// @return (float) the rotation
	float GetRotation() const { return m_Rotation; }

    /// @brief  sets the rotation
    /// @param  rotation    the new rotation
	void SetRotation( float rotation ) { m_Rotation = rotation; MarkChanged(); }


    /// @brief  gets the scale
    /// @return (glm::vec3) the scale
    glm::vec2 const& GetScale() const { return m_Scale; }

    /// @brief  sets the scale
    /// @param  scale   the new scale
    void SetScale( glm::vec2 const& scale ) { m_Scale = scale; MarkChanged(); }


    /// @brief  gets the isDiagetic flag
    /// @return (bool)  the isDiagetic flag
    bool const& GetIsDiegetic() const { return m_IsDiegetic; }

    /// @brief  sets the isDiagetic flag
    /// @param  isDiagetic  the new isDiagetic flag
    void SetIsDiegetic( bool const& isDiagetic ) { m_IsDiegetic = isDiagetic; }


    /// @brief  sets the dirty flag of this Transform
    /// @param  dirty   whether this transform is dirty
    void SetIsDirty( bool dirty ) { m_IsDirty = true; }


    /// @brief  gets the matrix
    /// @return (glm::mat4) the matrix
    glm::mat4 const& GetMatrix() const;

    /// @brief  sets the matrix
    /// @param  matrix  the new matrix
    void SetMatrix( glm::mat4 const& matrix );

//-----------------------------------------------------------------------------
private: // virtual override methods
//-----------------------------------------------------------------------------

    /// @brief  inspector for this component
    virtual void Inspector() override;

//-----------------------------------------------------------------------------
private: // member variables
//-----------------------------------------------------------------------------

    /// @brief  the position of this Transform
    glm::vec2 m_Translation = { 0.0f, 0.0f };

    /// @brief  the scale of this Transform
    glm::vec2 m_Scale = { 1.0f, 1.0f };

    /// @brief  the rotation of this Transform
    float m_Rotation = 0.0f;

    /// @brief  the matrix for this Transform
    glm::mat4 m_Matrix = glm::mat4(1);

    /// @brief  flag for when the matrix needs to be regenerated
    bool m_IsDirty = true;

	/// @brief  whether this Transform exists in world or screen space
	bool m_IsDiegetic = false;

	/// @brief  the Transform that this Transform descends from
	Transform* m_Parent = nullptr;

    /// @brief  callbacks to be called whenever this Transform changes
    std::map< unsigned, std::function< void () > > m_OnTransformChangedCallbacks = {};

//-----------------------------------------------------------------------------
private: // methods
//-----------------------------------------------------------------------------
    
    /// @brief  marks this Transform as dirty and calls the OnTransformChanged callbacks
    void MarkChanged();

//-----------------------------------------------------------------------------
private: // reading
//-----------------------------------------------------------------------------


    /// @brief  reads the translation
    /// @param  jsonValue   the json data to read from
    void readTranslation( nlohmann::ordered_json const& data );

    /// @brief  reads the rotation
    /// @param  jsonValue   the json data to read from
    void readRotation( nlohmann::ordered_json const& data );

    /// @brief  reads the scale
    /// @param  jsonValue   the json data to read from
    void readScale( nlohmann::ordered_json const& data );

    /// @brief  reads the isDiagetic flag
    /// @param  jsonValue   the json data to read from
    void readIsDiegetic( nlohmann::ordered_json const& data );

    /// @brief  called after finished reading
    virtual void AfterLoad() override;


    /// @brief  map of read methods
    static ReadMethodMap< Transform > s_ReadMethods;

    /// @brief  gets the map of read methods
    /// @return the map of read methods
    virtual ReadMethodMap< ISerializable > const& GetReadMethods() const override
    {
        return (ReadMethodMap< ISerializable > const&)s_ReadMethods;
    }

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
};
