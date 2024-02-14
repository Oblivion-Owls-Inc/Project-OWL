/// @file       Lifetime.cpp
/// @author     Steve Bukowinski (steve.bukowinski@digipen.edu)
/// @brief      Component that destroys it's Enity after a given length of time
/// @version    0.1
/// @date       2024-02-13
/// 
/// @copyright  Copyright (c) 2024 Digipen Institute of Technology

#include "Lifetime.h"

#include "BehaviorSystem.h"

#include "Engine.h"
#include "Entity.h"


//-----------------------------------------------------------------------------
// public: constructor / Destructor
//-----------------------------------------------------------------------------


    /// @brief  default constructor
    Lifetime::Lifetime() :
        Behavior( typeid( Lifetime ) )
    {}


//-----------------------------------------------------------------------------
// public: methods
//-----------------------------------------------------------------------------


//-----------------------------------------------------------------------------
// public: accessors
//-----------------------------------------------------------------------------


    /// @brief  gets this Lifetime's lifetime pool
    /// @return this Lifetime's lifetime pool
    Pool< float >* Lifetime::GetLifetime()
    {
        return &m_Lifetime;
    }


//-----------------------------------------------------------------------------
// public: virtual override methods
//-----------------------------------------------------------------------------


    /// @brief  called once when entering the scene
    void Lifetime::OnInit()
    {
        Behaviors< Behavior >()->AddComponent( this );
    }

    /// @brief  called once when exiting the scene
    void Lifetime::OnExit()
    {
        Behaviors< Behavior >()->RemoveComponent( this );
    }


    /// @brief  called every simulation frame
    void Lifetime::OnFixedUpdate()
    {
        m_Lifetime -= GameEngine()->GetFixedFrameDuration();

        if ( m_Lifetime.GetCurrent() <= 0.0f )
        {
            GetEntity()->Destroy();
        }
    }


//-----------------------------------------------------------------------------
// private: methods
//-----------------------------------------------------------------------------


//-----------------------------------------------------------------------------
// public: inspection
//-----------------------------------------------------------------------------


    /// @brief  shows the inspector for Lifetime
    void Lifetime::Inspector()
    {
        m_Lifetime.Inspect();
    }


//-----------------------------------------------------------------------------
// private: reading
//-----------------------------------------------------------------------------


    /// @brief  reads the amount of time until this Component's Entity will be destroyed
    /// @param  data    the JSON data to read from
    void Lifetime::readLifetime( nlohmann::ordered_json const& data )
    {
        Stream::Read( m_Lifetime, data );
    }


//-----------------------------------------------------------------------------
// public: reading / writing
//-----------------------------------------------------------------------------


    /// @brief  gets the map of read methods for this Lifetime
    /// @return the map of read methods for this Lifetime
    ReadMethodMap< ISerializable > const& Lifetime::GetReadMethods() const
    {
        static ReadMethodMap< Lifetime > const readMethods = {
            { "Lifetime", &Lifetime::readLifetime }
        };

        return (ReadMethodMap< ISerializable > const&)readMethods;
    }


    /// @brief  Writes all Lifetime data to a JSON file.
    /// @return The JSON file containing the data.
    nlohmann::ordered_json Lifetime::Write() const
    {
        nlohmann::ordered_json json;

        json[ "Lifetime" ] = Stream::Write( m_Lifetime );

        return json;
    }

    
//-----------------------------------------------------------------------------
// public: copying
//-----------------------------------------------------------------------------


    /// @brief  clones this Lifetime
    /// @return the newly created clone of this Lifetime
    Lifetime* Lifetime::Clone() const
    {
        return new Lifetime( *this );
    }


//-----------------------------------------------------------------------------
// private: copying
//-----------------------------------------------------------------------------


    /// @brief  copy-constructor for the Lifetime
    /// @param  other   the other Lifetime to copy
    Lifetime::Lifetime( Lifetime const& other ) :
        Behavior( other )
    {}


//-----------------------------------------------------------------------------
