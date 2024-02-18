///*****************************************************************/
/// @file	    HomeBase.cpp
/// @author     Tyler Birdsall (tyler.birdsall@digipen.edu)
/// @date	    10/27/2023
/// @brief      HomeBase class implimentation
/// @copyright  Digipen LLC (c) 2023
///*****************************************************************/

#include "HomeBase.h"

#include "SceneSystem.h"

//-----------------------------------------------------------------------------
// public: constructor
//-----------------------------------------------------------------------------


    /// @brief  constructor
    HomeBase::HomeBase() :
        Component( typeid( HomeBase ) )
    {}


//-----------------------------------------------------------------------------
// public: methods
//-----------------------------------------------------------------------------


    /// @brief	destroy the base
    void HomeBase::Destroy()
    {
	    SceneSystem::GetInstance()->SetNextScene( "Gameover" );
    }

    
//-----------------------------------------------------------------------------
// public: virtual override methods
//-----------------------------------------------------------------------------

    
    /// @brief  called once when entering the scene
    void HomeBase::OnInit()
    {
        m_Health.SetOnConnectCallback(
            [ this ]()
            {
                m_Health->AddOnHealthChangedCallback(
                    GetId(),
                    [ this ]()
                    {
                        if ( m_Health->GetHealth()->GetCurrent() <= 0 )
                        {
                            Destroy();
                        }
                    }
                );
            }
        );
        m_Health.SetOnDisconnectCallback(
            [ this ]()
            {
                m_Health->RemoveOnHealthChangedCallback( GetId() );
            }
        );

        m_Health.Init( GetEntity() );
    }

    /// @brief  called once when exiting the scene
    void HomeBase::OnExit()
    {
        m_Health.Exit();
    }


//-----------------------------------------------------------------------------
// public: inspection
//-----------------------------------------------------------------------------


    /// @brief  inspector for this HomeBase
    void HomeBase::Inspector()
    {
        if ( m_Health == nullptr )
        {
            ImGui::Text( "WARNING: no Health Component attached" );
        }

        ImGui::InputText( "game over scene name", &m_GameOverSceneName );
    }


//-----------------------------------------------------------------------------
// private: reading
//-----------------------------------------------------------------------------

    
    /// @brief  reads the name of the scene to transition to when the base dies
    /// @param  data    the JSON data to read from
    void HomeBase::readGameOverSceneName( nlohmann::ordered_json const& data )
    {
        Stream::Read( m_GameOverSceneName, data );
    }

    
//-----------------------------------------------------------------------------
// public: reading / writing
//-----------------------------------------------------------------------------


    /// @brief  gets the map of read methods for this HomeBase
    /// @return the map of read methods for this HomeBase
    ReadMethodMap< ISerializable > const& HomeBase::GetReadMethods() const
    {
        static ReadMethodMap< HomeBase > const readMethods = {
            { "GameOverScene", &HomeBase::readGameOverSceneName }
        };

        return (ReadMethodMap< ISerializable > const&)readMethods;

    }


    /// @brief  Writes all HomeBase data to a JSON file.
    /// @return The JSON file containing the data.
    nlohmann::ordered_json HomeBase::Write() const
    {
        nlohmann::ordered_json json;

        json[ "GameOverScene" ] = Stream::Write( m_GameOverSceneName );

        return json;
    }


//-----------------------------------------------------------------------------
// public: copying
//-----------------------------------------------------------------------------


    /// @brief  clone
    HomeBase* HomeBase::Clone() const
    {
        return new HomeBase( *this );
    }

    
//-----------------------------------------------------------------------------
// private: copying
//-----------------------------------------------------------------------------
    

    /// @brief  copy constructor
    HomeBase::HomeBase( HomeBase const& other ) :
        Component( other )
    {}


//-----------------------------------------------------------------------------
