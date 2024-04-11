///*****************************************************************/
/// @file	 RigidBody.cpp
/// @Author  Jax Clayton (jax.clayton@digipen.edu)
/// @date	 9/15/2021
/// @brief   RigidBody  
/// @details Contains Basic Physics for the Entities
///*****************************************************************/
/// 
 

#include "pch.h" // precompiled header has to be included first
#include "RigidBody.h"
#include "BehaviorSystem.h"
#include "DebugSystem.h"
#include "Engine.h"

#include "ComponentReference.t.h"
#include "Transform.h"
#include "Collider.h"
#include "StaticBody.h"

//-----------------------------------------------------------------------------
// public: constructor / destructors
//-----------------------------------------------------------------------------

    /// @brief Default constructor for the RigidBody class.
    RigidBody::RigidBody() : 
        Behavior( typeid( RigidBody ) )
    {}

//-----------------------------------------------------------------------------
// public: methods
//-----------------------------------------------------------------------------


    /// @brief  applies an acceleration to this RigidBody this frame
    /// @param  acceleration    the acceleration to apply
    void RigidBody::ApplyAcceleration( glm::vec2 const& acceleration )
    {
        m_Velocity += acceleration * GameEngine()->GetFixedFrameDuration();
    }

    /// @brief  adds to the Velocity of this Rigidbody
    /// @param  velocity    the velocity to apply
    void RigidBody::ApplyVelocity( glm::vec2 const& velocity )
    {
        m_Velocity += velocity;
    }


    /// @brief  applies a force to this RigidBody this frame
    /// @param  force   the force to apply
    void RigidBody::ApplyForce( glm::vec2 const& force )
    {
        ApplyImpulse( force * GameEngine()->GetFixedFrameDuration() );
    }

    /// @brief  applies an impulse to this RigidBody this frame
    /// @param  impulse the impulse to apply
    void RigidBody::ApplyImpulse( glm::vec2 const& impulse )
    {
        m_Velocity += impulse / m_Mass;
    }


//-----------------------------------------------------------------------------
// public: virtual override methods
//-----------------------------------------------------------------------------


    /// @brief Default constructor for the RigidBody class.
    void RigidBody::OnInit()
    {
        BehaviorSystem< RigidBody >::GetInstance()->AddComponent( this );

        m_Collider.SetOnConnectCallback(
            [ this ]()
            {
                m_Collider->AddOnCollisionCallback(
                    GetId(),
                    std::bind( &RigidBody::OnCollision, this, std::placeholders::_1, std::placeholders::_2 )
                );
            }
        );
        m_Collider.SetOnDisconnectCallback(
            [ this ]()
            {
                m_Collider->RemoveOnCollisionCallback( GetId() );
            }
        );

        m_Transform.Init( GetEntity() );
        m_Collider .Init( GetEntity() );
    }

    /// @brief  called when this Component's Entity is removed from the Scene
    /// @note   NOT CALLED WHEN THE SCENE IS EXITED - that should be handled by this Component's System
    void RigidBody::OnExit()
    {
        BehaviorSystem< RigidBody >::GetInstance()->RemoveComponent( this );

        m_Transform.Exit();
        m_Collider .Exit();
    }

    /// @brief Update method called per frame.
    /// @param dt The time elapsed since the last frame.
    void RigidBody::OnUpdate(float dt)
    {
        if ( m_Transform == nullptr )
        {
            return;
        }

        m_Transform->SetMatrix( glm::translate( glm::mat4( 1.0f ), glm::vec3( m_Velocity * dt, 0.0f ) ) * m_Transform->GetMatrix() );
    }

    /// @brief Fixed update method called at a fixed time step.
    void RigidBody::OnFixedUpdate()
    {
        if ( m_Transform == nullptr )
        {
            return;
        }

        float dt = GameEngine()->GetFixedFrameDuration();

        // linear movement
        glm::vec2 position = m_Transform->GetTranslation();
        m_Velocity += m_Acceleration * dt;
        position += m_Velocity * dt;

        // angular movement
        float rotation = m_Transform->GetRotation();
        rotation += m_RotationalVelocity * dt;

        // apply drag
        m_Velocity -= (m_Velocity * m_Drag * dt) / m_Mass;
        // TODO: angular drag

        // apply movement
        m_Transform->Set( position, rotation );
    }

    /// @brief Used by the Debug System to display information about this Component
    void RigidBody::Inspector()
    {
        if ( m_Transform == nullptr )
        {
            ImGui::Text( "WARNING: no Transform attached" );
        }

        if ( m_Collider == nullptr )
        {
            ImGui::Text( "no Collider attached" );
        }

        ImGui::DragFloat2( "Velocity", &m_Velocity.x );

        ImGui::DragFloat2( "Acceleration", &m_Acceleration.x );

        ImGui::DragFloat( "Rotational Velocity", &m_RotationalVelocity );

        ImGui::DragFloat( "Mass", &m_Mass, 0.05f, 0.05f, INFINITY );

        ImGui::DragFloat( "Restitution", &m_Restitution, 0.05f, 0.0f, 1.0f );

        ImGui::DragFloat( "Friction", &m_Friction, 0.05f, 0.0f, INFINITY );

        ImGui::DragFloat( "Drag", &m_Drag, 0.05f, 0.0f, INFINITY );
    }


//-----------------------------------------------------------------------------
// private: methods
//-----------------------------------------------------------------------------


    /// @brief  Called whenever a Collider on this Behavior's Entity collides
    /// @param  other           the collider that was collided with
    /// @param  collisionData   additional data about the collision
    void RigidBody::OnCollision( Collider* other, CollisionData const& collisionData )
    {
        // detect if colliding with StaticBody
        StaticBody const* staticBody = other->GetStaticBody();
        if ( staticBody != nullptr )
        {
            CollideWithStatic( staticBody, collisionData );
            return;
        }

        // only handle collisions with other rigidBodies
        RigidBody* rigidBodyB = other->GetRigidBody();
        if ( rigidBodyB == nullptr )
        {
            return;
        }
        // if the other rigidBody already handled this collision, don't handle it again
        if ( rigidBodyB->GetCollisionResolved() )
        {
            rigidBodyB->SetCollisionResolved( false );
            return;
        }

        Transform* transformB = other->GetTransform();


        // calculate new positions
        glm::vec2 position = m_Transform->GetTranslation();
        position += collisionData.normal * (collisionData.depth + 0.001f) * 0.5f;
        m_Transform->SetTranslation( position );

        position = transformB->GetTranslation();
        position -= collisionData.normal * (collisionData.depth + 0.001f) * 0.5f;
        transformB->SetTranslation( position );

        // get the appropriate variables from each body
        float massA = m_Mass;
        float massB = rigidBodyB->GetMass();
        float totalMass = massA + massB;

        glm::vec2 velA = m_Velocity;
        glm::vec2 velB = rigidBodyB->GetVelocity();

        // get speed in axis of collision
        float speedA = glm::dot( velA, collisionData.normal );
        float speedB = glm::dot( velB, collisionData.normal );


        // calculate the resulting velocities after the collision
        float relativeSpeed = speedB - speedA;
        float momentum = massA * speedA + massB * speedB;

        float restitution = m_Restitution * rigidBodyB->GetRestitution();

        float newSpeedA = (restitution * massB *  relativeSpeed + momentum) / totalMass;
        float newSpeedB = (restitution * massA * -relativeSpeed + momentum) / totalMass;

        // apply the new velocities in the axis of the collision normal
        velA += collisionData.normal * (newSpeedA - speedA);
        velB += collisionData.normal * (newSpeedB - speedB);


        this      ->SetVelocity( velA );
        rigidBodyB->SetVelocity( velB );

        // if other will also collide with this, mark the collision as already resolved
        if ( other->GetCollisionLayerFlags() & (1 << m_Collider->GetCollisionLayer()) )
        {
            m_CollisionResolved = true;
        }
    }


    /// @brief  resolve collision between this RigidBody and a StaticBody
    /// @param  other           the StaticBody to collide with
    /// @param  collisionData   additional data about the collision
    void RigidBody::CollideWithStatic( StaticBody const* other, CollisionData const& collisionData )
    {
        // move out of collision
        glm::vec2 pos = m_Transform->GetTranslation();
        pos += collisionData.normal * (collisionData.depth + 0.001f);
        m_Transform->SetTranslation( pos );

        float speed = glm::dot( m_Velocity, collisionData.normal );
        float newSpeed = -speed * m_Restitution * other->GetRestitution();
        float impulse = newSpeed - speed;
        
        glm::vec2 perpendicularAxis = glm::vec2( collisionData.normal.y, -collisionData.normal.x );
        float perpendicularSpeed = glm::dot( m_Velocity, perpendicularAxis );

        float frictionImpulse = m_Friction * other->GetFriction() * impulse;

        if ( frictionImpulse >= glm::abs( perpendicularSpeed ) )
        {
            m_Velocity += perpendicularAxis * -perpendicularSpeed;
        }
        else
        {
            m_Velocity += perpendicularAxis * frictionImpulse * -glm::sign( perpendicularSpeed );
        }

        m_Velocity += collisionData.normal * impulse;
    }

//-----------------------------------------------------------------------------
// private: reading
//-----------------------------------------------------------------------------

    /// @brief reads the velocity from json
    /// @param data the json data
    void RigidBody::readVelocity( nlohmann::ordered_json const& data )
    {
        Stream::Read( &m_Velocity, data );
    }

    /// @brief reads the acceleration from json
    /// @param data the json data
    void RigidBody::readAcceleration( nlohmann::ordered_json const& data )
    {
        Stream::Read( &m_Acceleration, data );
    }

    /// @brief reads the rotationalVelocity from json
    /// @param data the json data
    void RigidBody::readRotationalVelocity( nlohmann::ordered_json const& data )
    {
        Stream::Read( m_RotationalVelocity, data );
    }

    /// @brief reads the inverseMass from json
    /// @param data the json data
    void RigidBody::readMass( nlohmann::ordered_json const& data )
    {
        Stream::Read( m_Mass, data );
    }

    /// @brief reads the restitution from json
    /// @param data the json data
    void RigidBody::readRestitution( nlohmann::ordered_json const& data )
    {
        Stream::Read( m_Restitution, data );
    }

    /// @brief reads the friction from json
    /// @param data the json data
    void RigidBody::readFriction( nlohmann::ordered_json const& data )
    {
        Stream::Read( m_Friction, data );
    }

    /// @brief  reads te drag from json
    /// @param  data    the json data
    void RigidBody::readDrag( nlohmann::ordered_json const& data )
    {
        Stream::Read( m_Drag, data );
    }

    
//-----------------------------------------------------------------------------
// public: reading / writing
//-----------------------------------------------------------------------------


    /// @brief gets the map of read methods for this Component
    /// @return the map of read methods for this Component
    ReadMethodMap< ISerializable > const& RigidBody::GetReadMethods() const
    {
        static ReadMethodMap< RigidBody > const readMethods = {
            { "Velocity"          , &RigidBody::readVelocity           },
            { "Acceleration"      , &RigidBody::readAcceleration       },
            { "RotationalVelocity", &RigidBody::readRotationalVelocity },
            { "Mass"              , &RigidBody::readMass               },
            { "Restitution"       , &RigidBody::readRestitution        },
            { "Friction"          , &RigidBody::readFriction           },
            { "Drag"              , &RigidBody::readDrag               }
        };

        return (ReadMethodMap< ISerializable > const&)readMethods;
    }

    /// @brief  Write all RigidBody component data to a JSON file.
    /// @return The JSON file containing the RigidBody component data.
    nlohmann::ordered_json RigidBody::Write() const
    {
        nlohmann::ordered_json data;

        data[ "Velocity"           ] = Stream::Write( m_Velocity           );
        data[ "Acceleration"       ] = Stream::Write( m_Acceleration       );
        data[ "RotationalVelocity" ] = Stream::Write( m_RotationalVelocity );
        data[ "Mass"               ] = Stream::Write( m_Mass               );
        data[ "Restitution"        ] = Stream::Write( m_Restitution        );
        data[ "Friction"           ] = Stream::Write( m_Friction           );
        data[ "Drag"               ] = Stream::Write( m_Drag               );

        return data;
    }

    
//-----------------------------------------------------------------------------
// public: copying
//-----------------------------------------------------------------------------


    /// @brief  clones this RigidBody
    /// @return the newly created clone of this RigidBody
    RigidBody* RigidBody::Clone() const
    {
        return new RigidBody( *this );
    }

    
//-----------------------------------------------------------------------------
// private: copying
//-----------------------------------------------------------------------------


    /// @brief  copy-constructor for the RigidBody
    /// @param  other   the other RigidBody to copy
    RigidBody::RigidBody(const RigidBody& other) :
        Behavior( other ),
        m_Velocity          ( other.m_Velocity           ),
        m_Acceleration      ( other.m_Acceleration       ),
        m_RotationalVelocity( other.m_RotationalVelocity ),
        m_Mass              ( other.m_Mass               ),
        m_Restitution       ( other.m_Restitution        ),
        m_Friction          ( other.m_Friction           ),
        m_Drag              ( other.m_Drag               )
    {}


//-----------------------------------------------------------------------------
