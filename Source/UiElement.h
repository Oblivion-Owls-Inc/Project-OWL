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

//-----------------------------------------------------------------------------
// forward references
//-----------------------------------------------------------------------------


//-----------------------------------------------------------------------------
// class
//-----------------------------------------------------------------------------

class UiElement : public Transform
{
//-----------------------------------------------------------------------------
public: // constructor
//-----------------------------------------------------------------------------


    /// @brief  constructor
    UiElement();


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
public: // methods
//-----------------------------------------------------------------------------
    

    /// @brief  adds a child to this UiElement
    /// @param  child   the child to add
    void AddChild( UiElement* child );

    /// @brief  removes a child from this UiElement
    /// @param  child   the child to remove
    void RemoveChild( UiElement* child );


    /// @brief  adds a callback for whenever this UiElement gets clicked
    /// @param  ownerId     the ID of the owner of the callback
    /// @param  callback    the callback to call
    void AddOnClickedCallback( unsigned ownerId, std::function< void() > callback );

    /// @brief  removes a callback for whenever this UiElement gets clicked
    /// @param  ownerId     the ID of the owner of the callback
    void RemoveOnClickedCallback( unsigned ownerId );


//-----------------------------------------------------------------------------
public: // virtual override methods
//-----------------------------------------------------------------------------

    
    /// @brief  called once when entering the scene
    virtual void OnInit() override;

    /// @brief  called once when leaving the scene
    virtual void OnExit() override;


    /// @brief  displays the Inspector
    virtual void Inspector() override;


//-----------------------------------------------------------------------------
public: // accessors
//-----------------------------------------------------------------------------

    
    /// @brief  gets the parent UiElement
    /// @return the parent UiElement
    UiElement* GetParent() const { return m_Parent; }

    /// @brief  gets the children UiElements
    /// @return the chilfren UiElements
    std::vector< UiElement* > const& GetChildren() const { return m_Children; }


    /// @brief  gets the anchor position of this UiElement
    /// @return the anchor position
    glm::vec2 const& GetAnchor() const { return m_Anchor; }

    /// @brief  sets the anchor position of this UiElement
    /// @param  anchor  the anchor position
    void SetAnchor( glm::vec2 const& anchor ) { m_Anchor = anchor; updateTransform(); }


    /// @brief  gets the pivot position of this UiElement
    /// @return the pivot position
    glm::vec2 const& GetPivot() const { return m_Pivot; }

    /// @brief  sets the pivot position of this UiElement
    /// @param  pivot  the pivot position
    void SetPivot( glm::vec2 const& pivot ) { m_Pivot = pivot; updateTransform(); }


    /// @brief  gets the offset of this UiElement
    /// @return the offset
    glm::vec2 const& GetOffset() const { return m_Offset; }

    /// @brief  sets the offset of this UiElement
    /// @param  offset  the offset
    void SetOffset( glm::vec2 const& offset ) { m_Offset = offset; updateTransform(); }


    /// @brief  gets the frame size of this UiElement
    /// @return the frame size
    glm::vec2 const& GetFrameSize() const { return m_FrameSize; }

    /// @brief  sets the frame size of this UiElement
    /// @param  frameSize   the frame size
    void SetFrameSize( glm::vec2 const& frameSize ) { m_FrameSize = frameSize; updateTransform(); }


    /// @brief  gets the size types of this UiElement
    /// @return the size types
    SizeTypeVec const& GetSizeTypes() const { return m_SizeTypes; }

    /// @brief  sets the size types of this UiElement
    /// @param  sizeTypes   the size types
    void SetSizeTypes( SizeTypeVec const& sizeTypes ) { m_SizeTypes = sizeTypes; updateTransform(); }


//-----------------------------------------------------------------------------
private: // members
//-----------------------------------------------------------------------------


    /// @brief  the parent UiElement
    UiElement* m_Parent = nullptr;

    /// @brief  the children UiElements
    std::vector< UiElement* > m_Children = {};


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


    /// @brief  callbacks called whenever this UiElement is clicked
    std::map< unsigned, std::function< void() > > m_OnClickedCallbacks;


    /// @brief  the name of the Parent UiElement entity
    std::string m_ParentName = "";


//-----------------------------------------------------------------------------
// private: methods
//-----------------------------------------------------------------------------


    /// @brief  updates the Transform and all children transforms
    void updateTransform();


    /// @brief  call all callbacks
    void callOnClickedCallbacks();


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

    /// @brief  reads the ParentName
    /// @param  data    the json data
    void readParentName( nlohmann::ordered_json const& data );


    /// @brief  reads the SizeTypes
    /// @param  data    the json data
    void readSizeTypes( nlohmann::ordered_json const& data );

    /// @brief  map converting strings to size types
    static std::map< std::string, SizeType > const s_SizeTypes;


    /// @brief  map of read methods
    static ReadMethodMap< UiElement > const s_ReadMethods;


//-----------------------------------------------------------------------------
public: // reading / writing
//-----------------------------------------------------------------------------


    /// @brief  gets the map of read methods
    /// @return the map of read methods
    virtual ReadMethodMap< ISerializable > const& GetReadMethods() const override
    {
        return (ReadMethodMap< ISerializable > const&)s_ReadMethods;
    }


    /// @brief  writes this UiElement to json
    /// @return the written json data
    virtual nlohmann::ordered_json Write() const override;


//-----------------------------------------------------------------------------
public: // copying
//-----------------------------------------------------------------------------


    /// @brief  creates a new copy of this UiElement
    /// @return the newly created UiElement
    virtual UiElement* Clone() const override
    {
        return new UiElement( *this );
    }


//-----------------------------------------------------------------------------
private: // copying
//-----------------------------------------------------------------------------


    /// @brief  copy constructor
    /// @param  other   the other UiElement to copy
    UiElement( UiElement const& other );


//-----------------------------------------------------------------------------
};