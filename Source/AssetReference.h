/// @file       AssetReference.h
/// @author     Steve Bukowinski (steve.bukowinski@digipen.edu)
/// @brief      a reference to an Asset
/// @version    0.1
/// @date       2024-02-12
/// 
/// @copyright  Copyright (c) 2024 Digipen Institute of Technology

#pragma once

#define ASSETREFERENCE_H

#include "ISerializable.h"


/// @brief  a reference to an Asset
/// @tparam AssetType   the type of asset this AssetReference refers to
template < class AssetType >
class AssetReference : public ISerializable
{
//-----------------------------------------------------------------------------
public: // constructor
//-----------------------------------------------------------------------------


    /// @brief  constructor
    AssetReference();


    /// @brief  value contructor
    /// @param  asset   the asset this AssetReference will point to
    AssetReference( AssetType const* asset );


//-----------------------------------------------------------------------------
public: // methods
//-----------------------------------------------------------------------------


    /// @brief  initializes this AssetReference
    /// @param  required    whether this AssetReference is required (will not debug log if not required)
    void Init( bool required = true );


    /// @brief  sets this AssetReference to nullptr
    void Clear();


//-----------------------------------------------------------------------------
public: // accessors
//-----------------------------------------------------------------------------


    /// @brief  gets the name of the asset
    /// @return the name of the asset
    std::string const& GetName() const;


    /// @brief  sets this AssetReference's Owner Name
    /// @param  ownerName   the name of the owner of this AssetReference
    void SetOwnerName( std::string const& ownerName );


    /// @brief  dereference operator
    /// @return the Asset this AssetReference refers to
    AssetType const& operator *() const;

    /// @brief  member dereference operator
    /// @return the Asset this AssetReference refers to
    AssetType const* operator ->() const;

    /// @brief  implicit cast operator
    /// @return this AssetReference's internal pointer
    operator AssetType const*() const;


    /// @brief  AssetType* assignment operator
    /// @brief  NOTE: AssetReferences assigned this way cannot be serialized
    /// @param  asset   the asset to assign to this AssetReference
    void operator =( AssetType const* asset );

    
//-----------------------------------------------------------------------------
public: // copying
//-----------------------------------------------------------------------------


    /// @brief  assignment operator
    /// @param  other   the AssetReference to copy into this one
    void operator =( AssetReference const& other );

    /// @brief  copy constructor
    /// @param  other   the AssetReference to copy
    AssetReference( AssetReference const& other );


//-----------------------------------------------------------------------------
private: // members
//-----------------------------------------------------------------------------


    /// @brief  constant name used to denote non-serialized assets
    static constexpr char const* const s_NonSerializedName = "[non-serialized asset]";


    /// @brief  the name of the Asset this AssetReference points to
    std::string m_AssetName = "";

    /// @brief  the component this AssetReference is referring to
    AssetType const* m_Asset = nullptr;


    /// @brief  the name of the owner of this AssetReference (for debug logging)
    std::string m_OwnerName = "";

    
//-----------------------------------------------------------------------------
public: // inspection
//-----------------------------------------------------------------------------


    /// @brief  inspects this AssetReference
    /// @param  label   what to label this AssetReference inspector as
    /// @return whether this AssetReference was modified
    bool Inspect( char const* label );

    
//-----------------------------------------------------------------------------
private: // reading
//-----------------------------------------------------------------------------


    /// @brief  reads the name of the Asset this AssetReference points to
    /// @param  data    the JSON data to read from
    void readAssetName( nlohmann::ordered_json const& data );


//-----------------------------------------------------------------------------
public: // reading / writing
//-----------------------------------------------------------------------------


    /// @brief  gets the map of read methods for AssetReferences
    /// @return the map of read methods for AssetReferences
    virtual ReadMethodMap< ISerializable > const& GetReadMethods() const override;


    /// @brief  write all Asset data to JSON.
    /// @return the JSON containing the Asset data.
    virtual nlohmann::ordered_json Write() const override;


//-----------------------------------------------------------------------------
};

#ifndef ASSETREFERENCE_T
#include "AssetReference.t.cpp"
#endif
