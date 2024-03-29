/// @file       UiElement.h
/// @author     Steve Bukowinski (steve.bukowinski@digipen.edu)
/// @brief      Ui element component
/// @version    0.1
/// @date       2023-11-03
/// 
/// @copyright  Copyright (c) 2023 Digipen Institute of Technology

#pragma once

//-----------------------------------------------------------------------------
// includes
//-----------------------------------------------------------------------------

#include "Transform.h"

#include "ComponentReference.h"

//-----------------------------------------------------------------------------
// forward references
//-----------------------------------------------------------------------------


//-----------------------------------------------------------------------------
// class
//-----------------------------------------------------------------------------

class UiElement : public Transform
{
//-----------------------------------------------------------------------------
public: // types
//-----------------------------------------------------------------------------


    /// @brief  enum to specify what (if anything) the size is relative to)
    enum class SizeType
    {
        Absolute,
        RelativeToWidth,
        RelativeToHeight
    };

    /// @brief  used to specify what (if anything) each axis of the size is relative to
    typedef glm::vec< 2, SizeType > SizeTypeVec;


//-----------------------------------------------------------------------------
public: // constructor
//-----------------------------------------------------------------------------


    /// @brief  constructor
    UiElement();

    
//-----------------------------------------------------------------------------
public: // methods
//-----------------------------------------------------------------------------


    /// @brief  converts a UI position to a local anchor position
    /// @param  uiPos   the ui position to convert
    /// @return the converted local anchor position
    glm::vec2 UiPosToLocalAnchor( glm::vec2 const& uiPos ) const;


//-----------------------------------------------------------------------------
public: // accessors
//-----------------------------------------------------------------------------

    
    /// @brief  gets the parent UiElement
    /// @return the parent UiElement
    UiElement* GetParentElement() const;


    /// @brief  gets the anchor position of this UiElement
    /// @return the anchor position
    glm::vec2 const& GetAnchor() const;

    /// @brief  sets the anchor position of this UiElement
    /// @param  anchor  the anchor position
    void SetAnchor( glm::vec2 const& anchor );


    /// @brief  gets the pivot position of this UiElement
    /// @return the pivot position
    glm::vec2 const& GetPivot() const;

    /// @brief  sets the pivot position of this UiElement
    /// @param  pivot  the pivot position
    void SetPivot( glm::vec2 const& pivot );


    /// @brief  gets the offset of this UiElement
    /// @return the offset
    glm::vec2 const& GetOffset() const;

    /// @brief  sets the offset of this UiElement
    /// @param  offset  the offset
    void SetOffset( glm::vec2 const& offset );


    /// @brief  gets the frame size of this UiElement
    /// @return the frame size
    glm::vec2 const& GetFrameSize() const;

    /// @brief  sets the frame size of this UiElement
    /// @param  frameSize   the frame size
    void SetFrameSize( glm::vec2 const& frameSize );


    /// @brief  gets the size types of this UiElement
    /// @return the size types
    SizeTypeVec const& GetSizeTypes() const;

    /// @brief  sets the size types of this UiElement
    /// @param  sizeTypes   the size types
    void SetSizeTypes( SizeTypeVec const& sizeTypes );


//-----------------------------------------------------------------------------
public: // virtual override methods
//-----------------------------------------------------------------------------


    /// @brief  called once when entering the scene
    virtual void OnInit() override;

    /// @brief  called once when leaving the scene
    virtual void OnExit() override;


    /// @brief  called every time after the Entity this Component is attached to's heirarchy changes
    virtual void OnHierarchyChange( Entity* previousParent ) override;


//-----------------------------------------------------------------------------
private: // members
//-----------------------------------------------------------------------------


    /// @brief  what point on the parent is this UiElement relative to ( -1 to 1 )
    glm::vec2 m_Anchor = { 0, 0 };

    /// @brief  what point on this UiElement is relative to the parent ( -1 to 1 )
    glm::vec2 m_Pivot = { 0, 0 };

    /// @brief  absolute offset from the anchor to the pivot
    glm::vec2 m_Offset = { 0, 0 };


    /// @brief  the size of the UiElement (may be relative or absolute)
    glm::vec2 m_FrameSize = { 1, 1 };

    /// @brief  what the size of each axis is relative to
    SizeTypeVec m_SizeTypes = { SizeType::RelativeToWidth, SizeType::RelativeToHeight };


    /// @brief  the parent UiElement
    ComponentReference< UiElement > m_ParentElement;


    /// @brief  callbacks called whenever this UiElement is clicked
    std::map< unsigned, std::function< void() > > m_OnClickedCallbacks;


//-----------------------------------------------------------------------------
private: // methods
//-----------------------------------------------------------------------------


    /// @brief  updates the Transform and all children transforms
    void updateTransform();


    /// @brief  callback for whenever the window resizes
    void onWindowResizedCallback( glm::ivec2 const& );


//-----------------------------------------------------------------------------
public: // inspection
//-----------------------------------------------------------------------------


    /// @brief  displays the Inspector
    virtual void Inspector() override;


//-----------------------------------------------------------------------------
private: // reading
//-----------------------------------------------------------------------------
    

    /// @brief  reads the Anchor
    /// @param  data    the json data
    void readAnchor( nlohmann::ordered_json const& data );

    /// @brief  reads the Pivot
    /// @param  data    the json data
    void readPivot( nlohmann::ordered_json const& data );

    /// @brief  reads the Offset
    /// @param  data    the json data
    void readOffset( nlohmann::ordered_json const& data );

    /// @brief  reads the FrameSize
    /// @param  data    the json data
    void readFrameSize( nlohmann::ordered_json const& data );

    /// @brief  reads the SizeTypes
    /// @param  data    the json data
    void readSizeTypes( nlohmann::ordered_json const& data );


    /// @brief  map converting strings to size types
    static std::map< std::string, SizeType > const s_SizeTypes;


//-----------------------------------------------------------------------------
public: // reading / writing
//-----------------------------------------------------------------------------


    /// @brief  gets the map of read methods
    /// @return the map of read methods
    virtual ReadMethodMap< ISerializable > const& GetReadMethods() const override;


    /// @brief  writes this UiElement to json
    /// @return the written json data
    virtual nlohmann::ordered_json Write() const override;


//-----------------------------------------------------------------------------
public: // copying
//-----------------------------------------------------------------------------


    /// @brief  creates a new copy of this UiElement
    /// @return the newly created UiElement
    virtual UiElement* Clone() const override;


//-----------------------------------------------------------------------------
private: // copying
//-----------------------------------------------------------------------------


    /// @brief  copy constructor
    /// @param  other   the other UiElement to copy
    UiElement( UiElement const& other );


//-----------------------------------------------------------------------------
};