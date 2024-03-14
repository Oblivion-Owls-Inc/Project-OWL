/// @file       ActionReference.cpp
/// @author     Steve Bukowinski (steve.bukowinski@digipen.edu)
/// @brief      a reference to an input Action
/// @version    0.1
/// @date       2024-02-12
/// 
/// @copyright  Copyright (c) 2024 Digipen Institute of Technology

#include "pch.h" // precompiled header has to be included first

#include "ActionReference.h"

#include "DebugSystem.h"


//-----------------------------------------------------------------------------
// public: constructor
//-----------------------------------------------------------------------------


    /// @brief  constructor
    ActionReference::ActionReference()
    {}


//-----------------------------------------------------------------------------
// public: methods
//-----------------------------------------------------------------------------


    /// @brief  initializes this ActionReference
    void ActionReference::Init()
    {
        m_Action = Input()->GetActionByName( m_ActionName );
        if ( m_Action == nullptr )
        {
            Debug() << "WARNING: unable to find action with name \"" << m_ActionName
                << "\" (ActionReference owned by " << m_OwnerName << ")" << std::endl;
        }

        Input()->AddActionReference( this );
    }

    /// @brief  exits this ActionReference
    void ActionReference::Exit()
    {
        Input()->RemoveActionReference( this );
    }

    /// @brief  sets this ActionReference to nullptr
    void ActionReference::Clear()
    {
        m_Action = nullptr;
        m_ActionName = "";
    }


    /// @brief  assignment operator
    /// @param  action  the Action to assign to this ActionReference
    void ActionReference::operator =( Action const* action )
    {
        m_Action = action;
        m_ActionName = action == nullptr ? "" : action->GetName();
    }


//-----------------------------------------------------------------------------
// public: accessors
//-----------------------------------------------------------------------------


    /// @brief  gets the name of the asset
    /// @return the name of the asset
    std::string const& ActionReference::GetName() const
    {
        return m_ActionName;
    }


    /// @brief  gets this ActionReference's Owner Name
    /// @return this ActionReference's Owner Name
    std::string const& ActionReference::GetOwnerName() const
    {
        return m_OwnerName;
    }

    /// @brief  sets this ActionReference's Owner Name
    /// @param  ownerName   the name of the owner of this ActionReference
    void ActionReference::SetOwnerName( std::string const& ownerName )
    {
        m_OwnerName = ownerName;
    }


    /// @brief  dereference operator
    /// @return the Action this ActionReference refers to
    Action const& ActionReference::operator *() const
    {
        return *m_Action;
    }

    /// @brief  member dereference operator
    /// @return the Action this ActionReference refers to
    Action const* ActionReference::operator ->() const
    {
        return m_Action;
    }

    /// @brief  implicit cast operator
    /// @return the Action this ActionReference refers to
    ActionReference::operator Action const*() const
    {
        return m_Action;
    }

    
//-----------------------------------------------------------------------------
// public: copying
//-----------------------------------------------------------------------------


    /// @brief  assignment operator
    /// @param  other   the ActionReference to copy into this one
    void ActionReference::operator =( ActionReference const& other )
    {
        m_ActionName = other.m_ActionName;
        m_Action = other.m_Action;
        m_OwnerName = other.m_OwnerName;
    }

    /// @brief  copy constructor
    /// @param  other   the ActionReference to copy
    ActionReference::ActionReference( ActionReference const& other ) = default;

    
//-----------------------------------------------------------------------------
// public: inspection
//-----------------------------------------------------------------------------


    /// @brief  inspects this ActionReference
    /// @param  label   what to label this ActionReference inspector as
    /// @return whether this ActionReference was modified
    bool ActionReference::Inspect( char const* label )
    {
        if ( ImGui::BeginCombo( label, m_ActionName.c_str() ) )
        {
            if ( ImGui::Selectable( "[ none ]", m_Action == nullptr ) )
            {
                m_Action = nullptr;
                m_ActionName.clear();
                
                ImGui::EndCombo();
                return true;
            }

            for ( Action const& action : Input()->GetActions() )
            {
                if ( ImGui::Selectable( action.GetName().c_str(), m_Action == &action ) )
                {
                    m_Action = &action;
                    m_ActionName = action.GetName();

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


    /// @brief  reads the name of the Action this ActionReference points to
    /// @param  data    the JSON data to read from
    void ActionReference::readActionName( nlohmann::ordered_json const& data )
    {
        Stream::Read( m_ActionName, data );
    }


//-----------------------------------------------------------------------------
// public: reading / writing
//-----------------------------------------------------------------------------


    /// @brief  gets the map of read methods for ActionReferences
    /// @return the map of read methods for ActionReferences
    ReadMethodMap< ISerializable > const& ActionReference::GetReadMethods() const
    {
        static ReadMethodMap< ActionReference > const readMethods ={
            { "ActionName", &ActionReference::readActionName }
        };

        return (ReadMethodMap< ISerializable > const&)readMethods;
    }


    /// @brief  write all ActionReference data to JSON.
    /// @return the JSON containing the ActionReference data.
    nlohmann::ordered_json ActionReference::Write() const
    {
        nlohmann::ordered_json json;

        json[ "ActionName" ] = Stream::Write( m_ActionName );

        return json;
    }


//-----------------------------------------------------------------------------
