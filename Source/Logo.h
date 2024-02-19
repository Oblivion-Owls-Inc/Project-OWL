///--------------------------------------------------------------------------//
/// @file   Logo.h
/// @brief  Logo Behaviour Class
/// 
/// @author Aidan Straker (aidan.straker)
/// @date   February 2024
///
/// @copyright (c) 2024 DigiPen (USA) Corporation.
///--------------------------------------------------------------------------//


#pragma once
//------------------------------------------------------------------------------
// Includes:
//------------------------------------------------------------------------------
#include "Behavior.h"

#include <string>
#include <vector>
#include "ComponentReference.h"
#include "RigidBody.h"
#include "Transform.h"
#include "Sprite.h"
#include "EntityReference.h"
#include "AssetReference.h"


class SplashScreenController : public Behavior
{
//-----------------------------------------------------------------------------
private: // class LogoData
//-----------------------------------------------------------------------------

    // A sub class for logo data.
    class LogoData : ISerializable
    {
    //-----------------------------------------------------------------------------
    public:
    //-----------------------------------------------------------------------------

        /// @brief How long the logo is displayed for.
        float m_LogoTimer = 1.0f;

        /// @brief  The aspect ratio of the logo.
        float m_LogoAspectRatio = 1.0f;

        /// @brief The texture for the logo.
        AssetReference<Texture> m_LogoTexture;
        
    //-----------------------------------------------------------------------------
    public: // inspection
    //-----------------------------------------------------------------------------
    

        /// @brief  inspects this LogoData
        /// @return whether the LogoData was modified
        bool Inspect();


    //-----------------------------------------------------------------------------
    private: // Reading
    //-----------------------------------------------------------------------------


        /// @brief Read in the timer for the logo.
        /// @param data The JSON file to read from.
        void readTimer(nlohmann::ordered_json const& data);

        /// @brief Read in the aspect ratio for the logo.
        /// @param data The JSON file to read from. 
        void readAspectRatio(nlohmann::ordered_json const& data);

        /// @brief Read in the logos to de displayed.
        /// @param data The JSON file to read from.
        void readLogo(nlohmann::ordered_json const& data);

    //-----------------------------------------------------------------------------
    public: // reading / writing
    //-----------------------------------------------------------------------------


        /// @brief  Gets the map of read methods for this Component
        /// @return The map of read methods for this Component
        virtual ReadMethodMap< ISerializable > const& GetReadMethods() const override;


        /// @brief  Write all Logo behavior data to a JSON file.
        /// @return The JSON file containing the Logo behavior data.
        virtual nlohmann::ordered_json Write() const override;


    //-----------------------------------------------------------------------------
    };

//-----------------------------------------------------------------------------
public: // constructor/destructor
//-----------------------------------------------------------------------------
    
    /// @brief Default constructor for the logo behaviour.
    SplashScreenController();

    /// @brief Destructor for the logo behaviour.
    ~SplashScreenController() = default;

//-----------------------------------------------------------------------------
public: // virtual override methods
//-----------------------------------------------------------------------------

    /// @brief Called when this component's entity is added to the scene
    virtual void OnInit() override;

    /// @brief Called when this component's entity is removed from the scene
    virtual void OnExit() override;

    /// @brief Update method called per frame
    virtual void OnFixedUpdate() override;

//-----------------------------------------------------------------------------
private: // member variables
//-----------------------------------------------------------------------------

    /// @brief The scene to switch too.
    std::string m_NextSceneName = "";

    /// @brief  A cached instance of the parent's Rigidbody.
    ComponentReference<RigidBody> m_RigidBody;

    /// @brief  A cached instance of the parent's Sprite.
    ComponentReference<Sprite> m_Sprite;

    /// @brief  A cached instance of the parent's Transform.
    ComponentReference<Transform> m_Transform;

    /// @brief The logos to be displayed.
    std::vector<LogoData> m_Logos = {};

    int m_Index = 0;

//-----------------------------------------------------------------------------
private: // methods
//-----------------------------------------------------------------------------

    /// @brief Switches the logo to be displayed.
    void SwitchLogo();

//-----------------------------------------------------------------------------
public: // Inspection
//-----------------------------------------------------------------------------

    /// @brief Used by the Debug System to display information about this Component
    virtual void Inspector() override;

//-----------------------------------------------------------------------------
private: // Reading
//-----------------------------------------------------------------------------

    /// @brief Read in the name of the scene to skip to
    /// @param data The JSON to read from.
    void readSceneName(nlohmann::ordered_json const& data);

    /// @brief Read in the logo data for each logo
    /// @param data the JSON fie to read from.
    void readLogos(nlohmann::ordered_json const& data);

//-----------------------------------------------------------------------------
public: // reading / writing
//-----------------------------------------------------------------------------


    /// @brief  Gets the map of read methods for this Component
    /// @return The map of read methods for this Component
    virtual ReadMethodMap< ISerializable > const& GetReadMethods() const override;


    /// @brief  Write all Logo behavior data to a JSON file.
    /// @return The JSON file containing the Logo behavior data.
    virtual nlohmann::ordered_json Write() const override;


//-----------------------------------------------------------------------------
public: // copying
//-----------------------------------------------------------------------------


    /// @brief  Creates a new copy of this Component.
    /// @return The newly created component
    virtual SplashScreenController* Clone() const override;


//-----------------------------------------------------------------------------
private: // copying
//-----------------------------------------------------------------------------


    /// @brief  Copy constructor.
    SplashScreenController(SplashScreenController const& other);


    // disable assignment operator 
    void operator =(SplashScreenController const&) = delete;


//-----------------------------------------------------------------------------

};

