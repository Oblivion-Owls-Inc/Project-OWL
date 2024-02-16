/// @file       AssetReference.t.cpp
/// @author     Steve Bukowinski (steve.bukowinski@digipen.edu)
/// @brief      a reference to an Asset
/// @version    0.1
/// @date       2024-02-12
/// 
/// @copyright  Copyright (c) 2024 Digipen Institute of Technology

#define ASSETREFERENCE_T

#ifndef ASSETREFERENCE_H
#include "AssetReference.h"
#endif

#include "AssetLibrarySystem.h"


/// @brief  a reference to an Asset
/// @tparam AssetType   the type of asset this AssetReference refers to
//-----------------------------------------------------------------------------
// public: constructor
//-----------------------------------------------------------------------------


    /// @brief  constructor
    /// @tparam AssetType   the type of asset this AssetReference refers to
    template< class AssetType >
    AssetReference< AssetType >::AssetReference()
    {}


//-----------------------------------------------------------------------------
// public: methods
//-----------------------------------------------------------------------------


    /// @brief  initializes this AssetReference
    /// @tparam AssetType   the type of asset this AssetReference refers to
    template< class AssetType >
    void AssetReference< AssetType >::Init()
    {
        m_Asset = AssetLibrary< AssetType >()->GetAsset( m_AssetName );

        #ifdef _DEBUG
        if ( m_Asset == nullptr )
        {
            Debug() << "Warning: Could not find asset of type \"" << PrefixlessName( typeid( AssetType ) )
                << "\" with name \"" << m_AssetName
                << "\" (AssetReference owned by \"" << m_OwnerName << "\")\n" << std::endl;
        }
        #endif
    }


    /// @brief  sets this AssetReference to nullptr
    /// @tparam AssetType   the type of asset this AssetReference refers to
    template< class AssetType >
    void AssetReference< AssetType >::Clear()
    {
        m_Asset = nullptr;
        m_AssetName.clear();
    }


//-----------------------------------------------------------------------------
// public: accessors
//-----------------------------------------------------------------------------


    /// @brief  gets the name of the asset
    /// @tparam AssetType   the type of asset this AssetReference refers to
    /// @return the name of the asset
    template< class AssetType >
    std::string const& AssetReference< AssetType >::GetName() const
    {
        return m_AssetName;
    }


    /// @brief  sets this AssetReference's Owner Name
    /// @tparam AssetType   the type of asset this AssetReference refers to
    /// @param  ownerName   the name of the owner of this AssetReference
    template< class AssetType >
    void AssetReference< AssetType >::SetOwnerName( std::string const& ownerName )
    {
        m_OwnerName = ownerName;
    }


    /// @brief  dereference operator
    /// @tparam AssetType   the type of asset this AssetReference refers to
    /// @return the Component this AssetReference refers to
    template< class AssetType >
    AssetType const& AssetReference< AssetType >::operator *() const
    {
        return *m_Asset;
    }

    /// @brief  member dereference operator
    /// @tparam AssetType   the type of asset this AssetReference refers to
    /// @return the Component this AssetReference refers to
    template< class AssetType >
    AssetType const* AssetReference< AssetType >::operator ->() const
    {
        return m_Asset;
    }

    /// @brief  implicit cast operator
    /// @tparam AssetType   the type of asset this AssetReference refers to
    /// @return this AssetReference's internal pointer
    template< class AssetType >
    AssetReference< AssetType >::operator AssetType const*() const
    {
        return m_Asset;
    }


    /// @brief  AssetType* assignment operator
    /// @brief  NOTE: AssetReferences assigned this way cannot be serialized
    /// @tparam AssetType   the type of asset this AssetReference refers to
    /// @param  asset   the asset to assign to this AssetReference
    template< class AssetType >
    void AssetReference< AssetType >::operator =( AssetType const* asset )
    {
        m_Asset = asset;
        m_AssetName.clear();
    }

    
//-----------------------------------------------------------------------------
// public: copying
//-----------------------------------------------------------------------------


    /// @brief  assignment operator
    /// @tparam AssetType   the type of asset this AssetReference refers to
    /// @param  other   the AssetReference to copy into this one
    template< class AssetType >
    void AssetReference< AssetType >::operator =( AssetReference const& other )
    {
        m_Asset = other.m_Asset;
        m_AssetName = other.m_AssetName;
    }

    /// @brief  copy constructor
    /// @tparam AssetType   the type of asset this AssetReference refers to
    /// @param  other   the AssetReference to copy
    template< class AssetType >
    AssetReference< AssetType >::AssetReference( AssetReference const& other )
    {
        *this = other;
    }

    
//-----------------------------------------------------------------------------
// public: inspection
//-----------------------------------------------------------------------------


    /// @brief  inspects this AssetReference
    /// @tparam AssetType   the type of asset this AssetReference refers to
    /// @param  label   what to label this AssetReference inspector as
    /// @return whether this AssetReference was modified
    template< class AssetType >
    bool AssetReference< AssetType >::Inspect( char const* label )
    {
        if ( ImGui::BeginCombo( label, m_AssetName.c_str() ) )
        {
            for ( auto const& [ name, asset ] : AssetLibrary< AssetType >()->GetAssets() )
            {
                if ( ImGui::Selectable( name.c_str(), asset == m_Asset ) )
                {
                    m_Asset = asset;
                    m_AssetName = name;

                    ImGui::EndCombo();
                    return true;
                }
            }

            ImGui::EndCombo();
        }

        return false;
    }

    
//-----------------------------------------------------------------------------
// private: reading
//-----------------------------------------------------------------------------


    /// @brief  reads the name of the Asset this AssetReference points to
    /// @tparam AssetType   the type of asset this AssetReference refers to
    /// @param  data    the JSON data to read from
    template< class AssetType >
    void AssetReference< AssetType >::readAssetName( nlohmann::ordered_json const& data )
    {
        Stream::Read( m_AssetName, data );
    }


//-----------------------------------------------------------------------------
// public: reading / writing
//-----------------------------------------------------------------------------


    /// @brief  gets the map of read methods for Entities
    /// @tparam AssetType   the type of asset this AssetReference refers to
    /// @return the map of read methods for Entities
    template< class AssetType >
    ReadMethodMap< ISerializable > const& AssetReference< AssetType >::GetReadMethods() const
    {
        static ReadMethodMap< AssetReference > const readMethods = {
            { "AssetName", &AssetReference::readAssetName }
        };

        return (ReadMethodMap< ISerializable > const&)readMethods;
    }


    /// @brief  Write all Asset data to a JSON file.
    /// @tparam AssetType   the type of asset this AssetReference refers to
    /// @return The JSON file containing the Asset data.
    template< class AssetType >
    nlohmann::ordered_json AssetReference< AssetType >::Write() const
    {
        nlohmann::ordered_json json;

        json[ "AssetName" ] = Stream::Write( m_AssetName );

        return json;
    }


//-----------------------------------------------------------------------------
