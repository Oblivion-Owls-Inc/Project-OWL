///*****************************************************************/
/// @file SteamAchievements.h
/// @author Jax Clayton (jax.clayton@digipen.edu)
/// 
/// @brief This is a file that defines the SteamAchievements struct that is used to store the achievements for the game
/// 
/// @copyright  Copyright (c) 2024 Digipen Institute of Technology
///*****************************************************************/

#pragma once
#include "pch.h"

// Define a constant for the achievement ID and name
 #define ACH_ID(id, name) { id, #id, name, "", false, 0 }

#define APPID int

// Define a struct for representing an achievement
class Achievement : public ISerializable
{

//-----------------------------------------------------------------------------
public: // constructor / Destructor
//-----------------------------------------------------------------------------

    /// //////////////////////////////////////////////////////////////////////////////////////////////
    /// @brief Constructor for the Achievement struct
    /// @param id - The achievement ID
    /// @param achievementID - The achievement ID as a string
    /// @param name - The name of the achievement
    /// @param description - The description of the achievement
    /// @param achieved - Whether the achievement has been achieved
    /// @param iconImage - The icon image for the achievement
    /// ////////////////////////////////////////////////////////////////////////////////////////////
    Achievement( int id, const std::string& achievementID, const std::string& name,
        const std::string& description, bool achieved, int iconImage )
        : m_eAchievementID(id), 
        m_pchAchievementID(achievementID),
        m_rgchName(name),
        m_rgchDescription(description), 
        m_bAchieved(achieved), 
        m_iIconImage(iconImage) {}

///-----------------------------------------------------------------------------
private: // reading
///-----------------------------------------------------------------------------

    void readAchievementID( nlohmann::ordered_json const& json )
    {
        Stream::Read(m_eAchievementID, json);
    }

    void readpchAchievementID( nlohmann::ordered_json const& json )
    {
        Stream::Read(m_pchAchievementID, json);
    }

    void readrgchName( nlohmann::ordered_json const& json )
    {
        Stream::Read(m_rgchName, json);
    }

    void readrgchDescription( nlohmann::ordered_json const& json )
    {
        Stream::Read(m_rgchDescription, json);
    }

    void readbAchieved( nlohmann::ordered_json const& json )
    {
        Stream::Read(m_bAchieved, json);
    }

    void readiIconImage( nlohmann::ordered_json const& json )
    {
        Stream::Read(m_iIconImage, json);
    }


///-----------------------------------------------------------------------------
public: // reading / writing
///-----------------------------------------------------------------------------

    /// @brief  Gets the map of read methods for this object
    /// @return the map of read methods for htis object
    virtual ReadMethodMap< ISerializable > const& GetReadMethods() const
    {
        static ReadMethodMap< Achievement > const readMethods = 
        {
            { "AchievementID", &Achievement::readAchievementID },
            { "StringID", &Achievement::readpchAchievementID },
            { "Name", &Achievement::readrgchName },
            { "Description", &Achievement::readrgchDescription },
            { "Achieved", &Achievement::readbAchieved },
            { "IconImage", &Achievement::readiIconImage }
        };
        
        return (ReadMethodMap< ISerializable > const&)readMethods;
    }

    /// @brief  writes this object to json
    /// @return the json data of this object
    virtual nlohmann::ordered_json Write() const 
    {
        nlohmann::ordered_json json;
        json["AchievementID"] = m_eAchievementID;
        json["StringID"] = m_pchAchievementID;
        json["Name"] = m_rgchName;
        json["Description"] = m_rgchDescription;
        json["Achieved"] = m_bAchieved;
        json["IconImage"] = m_iIconImage;
        return json;
    }

///-----------------------------------------------------------------------------
private: // members
///-----------------------------------------------------------------------------

    int m_eAchievementID;             ///< The achievement ID
    std::string m_pchAchievementID;  ///< The achievement ID as a string
    std::string m_rgchName;         ///< The name of the achievement
    std::string m_rgchDescription; ///< The description of the achievement
    bool m_bAchieved;             ///< Whether the achievement has been achieved
    int m_iIconImage;            ///< The icon image for the achievement

///-----------------------------------------------------------------------------
private: /// copying
///-----------------------------------------------------------------------------


    /// @brief  copy-constructor for the ExampleComponent
    /// @param  other   the other ExampleComponent to copy
    Achievement(Achievement const& other );

    // disable assignment operator
    void operator =(Achievement const& ) = delete;
///-----------------------------------------------------------------------------
};