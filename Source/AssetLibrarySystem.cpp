/// @file AssetLibrarySystem.cpp
/// @author Tyler Birdsall (tyler.birdsall@digipen.edu)
/// @brief AssetLibrarySystem
/// @version 0.1
/// @date 2023-09-26
/// 
/// @copyright Copyright (c) 2023

#define ASSETLIBRARYSYSTEM_C

#ifndef ASSETLIBRARYSYSTEM_H
#include "AssetLibrarySystem.h"
#endif

#include "DebugSystem.h"

///----------------------------------------------------------------------------
/// 
///----------------------------------------------------------------------------
 template< class AssetType >
bool AssetLibrarySystem< AssetType >::s_ShowAssetLibraryList = false;



//-----------------------------------------------------------------------------
// virtual override methods
//-----------------------------------------------------------------------------

    /// @brief  Gets called whenever a scene is exited
    template< class AssetType >
    void AssetLibrarySystem< AssetType >::OnSceneExit()
    {
	    FlushAssets();
    }

    /// @brief  loads all assets of this AssetLibrary's type from JSON
    /// @param  data    the json data to load from
    template< class AssetType >
    void AssetLibrarySystem< AssetType >::LoadAssets( nlohmann::ordered_json const& data )
    {
        for ( auto& [ key, value ] : data.items() )
        {
            // create and read the asset
            AssetType* asset = new AssetType();
            Stream::Read( asset, value );

            // insert the asset into the map
            AddAsset( key, asset );
        }
    }

    /// @brief  saves all assets of this AssetLibrary's type to JSON
    /// @tparam AssetType   the type of asset
    /// @return the written json data
    template< class AssetType >
    nlohmann::ordered_json AssetLibrarySystem< AssetType >::SaveAssets() const
    {
        nlohmann::ordered_json json;

        for ( auto& [ key, value ] : m_Assets )
        {
            json[ key ] = value->Write();
        }

        return json;
    }

    template<class AssetType>
    void AssetLibrarySystem<AssetType>::DebugWindow()
    {
        s_ShowAssetLibraryList = GetDebugEnabled();
        std::string AssetName(typeid(AssetType).name() + 5); // skip over the "class" part of the name

        if (ImGui::Begin( AssetName.c_str(), &s_ShowAssetLibraryList ))
        {
            ImGui::SetWindowSize(ImVec2(500, 500), ImGuiCond_FirstUseEver);

                ListAssets();
        }

        ImGui::End();

        SetDebugEnable(s_ShowAssetLibraryList);
    }

//-----------------------------------------------------------------------------
// public: public functions
//-----------------------------------------------------------------------------


/// @brief  Finds and returns an asset, builds if doesn't yet exist
/// @return the constructed or found asset
template< class AssetType >
AssetType const* AssetLibrarySystem< AssetType >::GetAsset( std::string const& name ) const
{
    auto itr = m_Assets.find(name);
    if (itr != m_Assets.end())
    {
        return itr->second;
    }
    else
    {
        return nullptr;
    }
}

    /// @brief Finds an returns the name of the specified asset.
    /// @param asset The asset to search for.
    /// @return The name of the asset.
    template<class AssetType>
    char const* AssetLibrarySystem<AssetType>::GetAssetName( AssetType const* asset ) const
    {
        for ( auto& pair : m_Assets)
        {
            if (pair.second == asset)
            {
                return pair.first.c_str();
            }
        }

        return "";
    }


    /// @brief  Adds an asset to the AssetLibrary
    /// @param  name    the name of the asset to add
    /// @param  asset   the asset to add
    template< class AssetType >
    void AssetLibrarySystem< AssetType >::AddAsset( std::string const& name, AssetType* asset )
    {
        if (m_Assets.find(name) != m_Assets.end()) {
            Debug() << "Error: Asset with name '" << name << "' already exists." << std::endl;
        }
        m_Assets.insert( { name, asset } );
    }

    template<class AssetType>
    std::map<std::string, AssetType*> const& AssetLibrarySystem<AssetType>::GetAssets() const
    {
        return m_Assets;
    }

    /// @brief Used to create a new asset of the specified type.
    /// @tparam AssetType - the type of asset to create
    /// @return - If the window is open or not.
    template<class AssetType>
    bool AssetLibrarySystem<AssetType>::DebugCreateAssetWindow()
    {
        bool show = true;
        std::string assetName(typeid(AssetType).name() + 5); // skip over the "class" part of the name

        std::string windowName = "Create New " + assetName;

        ImGui::Begin(windowName.c_str(),&show, ImGuiWindowFlags_AlwaysAutoResize);
        ImGui::SetNextWindowSize(ImVec2(500, 100), ImGuiCond_FirstUseEver);
        // Input text for asset name
        static char buffer[128] = ""; // Buffer to hold the input, you can save this
        ImGui::PushItemWidth(ImGui::GetWindowWidth() * 0.45f);
        ImGui::InputText("##Asset Name", buffer, IM_ARRAYSIZE(buffer));

        // add asset button
        ImGui::SameLine();
        if (ImGui::Button("Add Asset", ImVec2(100, 0)))
        {
            if (buffer[0] == '\0')
            {
                Debug() << "Warning: Asset must have a name" << std::endl;
            }
            else
            {
                AddAsset(buffer, new AssetType());
                ImGui::End();
                return false; //close window
            }
        }
        ImGui::SameLine();

        if (ImGui::Button("Cancel", ImVec2(100, 0)))
        {
            ImGui::End();
			return false; //close window
        }

        if (!show)
        {
			ImGui::End();
			return false; //close window
		}

        ImGui::End();
        return true; // keep window open
    }

//-----------------------------------------------------------------------------
// private: private functions
//-----------------------------------------------------------------------------


    /// @brief  Flushes everything in the library
    /// @brief  Automatically called on scene exit
    template<class AssetType>
    void AssetLibrarySystem<AssetType>::FlushAssets()
    {
        for ( auto& key : m_Assets )
        {
            delete key.second;
        }
        m_Assets.clear();
    }

    template<class AssetType>
    void AssetLibrarySystem<AssetType>::ListAssets()
    {
        // list assets
        for (auto& key : m_Assets)
        {
            if ( !ImGui::TreeNode( key.first.c_str() ) )
            {
                continue;
            }

            key.second->Inspect();

            ImGui::TreePop();
        }
	    
    }


//-----------------------------------------------------------------------------
// singleton stuff
//-----------------------------------------------------------------------------


    /// @brief constructs the AssetLibrarySystem
    template< class AssetType >
    AssetLibrarySystem< AssetType >::AssetLibrarySystem() :
        BaseAssetLibrarySystem( std::string( "AssetLibrary<" ) + (typeid( AssetType ).name() + 6) + ">" )
    {
    }

    /// @brief the singleton instance of AssetLibrarySystem
    template< class AssetType >
    AssetLibrarySystem< AssetType >* AssetLibrarySystem< AssetType >::s_Instance = nullptr;


    /// @brief gets the instance of AssetLibrarySystem
    /// @return the instance of the AssetLibrarySystem
    template< class AssetType >
    AssetLibrarySystem< AssetType >* AssetLibrarySystem< AssetType >::GetInstance()
    {
        if ( s_Instance == nullptr )
        {
            s_Instance = new AssetLibrarySystem();
        }
        return s_Instance;
    }


//-----------------------------------------------------------------------------
