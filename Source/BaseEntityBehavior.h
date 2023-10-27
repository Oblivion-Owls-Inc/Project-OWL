#pragma once
#include "Behavior.h"
#include "Pool.h"


class BaseEntityBehavior :
    public Behavior
{

protected:

    BaseEntityBehavior(std::type_index m_Type) :
        Behavior(m_Type), m_Health("Health")
    {}

///-----------------------------------------------------------------------------
public: // accessors
///-----------------------------------------------------------------------------

    /// @brief Returns the health of the enemy
    /// @return - the health of the enemy
    Pool<int>* GetHealth() { return &m_Health; }

//-----------------------------------------------------------------------------
private: // virtual override methods
//-----------------------------------------------------------------------------

    /// @brief initializes the component
    virtual void OnInit() override;

    /// @brief Called when the component is destroyed
    virtual void OnExit() override;

    /// @brief Called at a fixed interval
    virtual void OnFixedUpdate() override {};

    /// @brief  inspector for this component
    virtual void Inspector() override;

///-----------------------------------------------------------------------------
protected: // Reading
///-----------------------------------------------------------------------------

    /// @brief  reads the health of the enemy from json
    /// @param  data    the json data to read from
    void readHealth(nlohmann::ordered_json const& data);

    /// @brief  map of read methods
    static ReadMethodMap< BaseEntityBehavior > const s_ReadMethods;

///-----------------------------------------------------------------------------
public: // reading / writing
///-----------------------------------------------------------------------------

    /// @brief  gets the map of read methods
    /// @return the map of read methods
    virtual ReadMethodMap< ISerializable > const& GetReadMethods() const override
    {
        return (ReadMethodMap< ISerializable > const&)s_ReadMethods;
    }

    /// @brief  write all component data to a JSON object
    /// @return the JSON object containing the component data
    nlohmann::ordered_json Write() const;   

///-----------------------------------------------------------------------------
public: // methods
///-----------------------------------------------------------------------------
    
    /// @brief Apply damage to the Entity 
    /// @param damage - the amount of damage to Entity
    void TakeDamage( int damage );


///-----------------------------------------------------------------------------
private: // Member Variables
///-----------------------------------------------------------------------------

    /// @brief  the Health of the Enemy
    Pool<int> m_Health;

///-------------------------------------------------------------------------------------------
public: // copying
///-------------------------------------------------------------------------------------------

    /// @brief  creates a new clone of this Component
    /// @return the newly created clone
    virtual BaseEntityBehavior* Clone() const override
    {
        return new BaseEntityBehavior( *this );
    }

///-------------------------------------------------------------------------------------------
private: // copying
///-------------------------------------------------------------------------------------------
    
    /// @brief  copy constructor
    /// @param  other   the BaseEntityBehavior to copy
    BaseEntityBehavior(BaseEntityBehavior const& other);

};

