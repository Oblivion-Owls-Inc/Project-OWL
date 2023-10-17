///*****************************************************************/
/// @file	 RigidBody.cpp
/// @Author  Jax Clayton (jax.clayton@digipen.edu)
/// @date	 9/15/2021
/// @brief   RigidBody  
/// @details Contains Basic Physics for the Entities
///*****************************************************************/
#include "RigidBody.h"
#include "BehaviorSystem.h"
#include "DebugSystem.h"
#include "Engine.h"

#include "Collider.h"
#include "Transform.h"

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
        m_Velocity += acceleration * Engine::GetInstance()->GetFixedFrameDuration();
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
        ApplyImpulse( force * Engine::GetInstance()->GetFixedFrameDuration() );
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
        BehaviorSystem<RigidBody>::GetInstance()->AddBehavior(this);
        m_Transform = GetParent()->GetComponent<Transform>();
        m_Collider = GetParent()->GetComponent<Collider>();
        
        m_OnCollisionCallbackHandle = m_Collider->AddOnCollisionCallback(
            std::bind( &RigidBody::OnCollision, this, std::placeholders::_1, std::placeholders::_2 )
        );
    }

    /// @brief  called when this Component's Entity is removed from the Scene
    /// @note   NOT CALLED WHEN THE SCENE IS EXITED - that should be handled by this Component's System
    void RigidBody::OnExit()
    {
        BehaviorSystem<RigidBody>::GetInstance()->RemoveBehavior(this);
        m_Collider->RemoveOnCollisionCallback( m_OnCollisionCallbackHandle );
    }

    /// @brief Update method called per frame.
    /// @param dt The time elapsed since the last frame.
    void RigidBody::OnUpdate(float dt)
    {
        // TODO: interpolate visual position
    }

    /// @brief Fixed update method called at a fixed time step.
    void RigidBody::OnFixedUpdate()
    {
	    float dt = Engine::GetInstance()->GetFixedFrameDuration();

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

    /// @brief  Called whenever a Collider on this Behavior's Entity collides
    /// @param  other           the collider that was collided with
    /// @param  collisionData   additional data about the collision
    void RigidBody::OnCollision( Collider* other, CollisionData const& collisionData )
    {

        // only handle collisions with other rigidBodies
        RigidBody* rigidBodyB = other->GetParent()->GetComponent<RigidBody>();
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

        // move the bodies away from each other so they no longer overlap
        glm::vec2 position = m_Transform->GetTranslation();
        position += collisionData.normal * collisionData.depth * 0.5f;
        m_Transform->SetTranslation(
            glm::vec3(position.x, position.y, 1.0f)
        );

        position = transformB->GetTranslation();
        position -= collisionData.normal * collisionData.depth * 0.5f;
        transformB->SetTranslation(
            glm::vec3(position.x, position.y, 1.0f)
        );

        // calculate the momentum and energy of the collision in the axis of the collision normal
        float massA = m_Mass;
        float massB = rigidBodyB->GetMass();
        float totalMass = massA + massB;

        glm::vec2 velA = m_Velocity;
        glm::vec2 velB = rigidBodyB->GetVelocity();

        float speedA = glm::dot( velA, collisionData.normal );
        float speedB = glm::dot( velB, collisionData.normal );
        float relativeSpeed = speedB - speedA;

        float momentum = massA * speedA + massB * speedB;

        float restitution = m_Restitution * rigidBodyB->GetRestitution();
        
        float newSpeedA = (restitution * massB * relativeSpeed + momentum) / totalMass;
        float newSpeedB = (restitution * massA * -relativeSpeed + momentum) / totalMass;

        // apply the new velocities in the axis of the collision normal
        velA += collisionData.normal * (newSpeedA - speedA);
        velB += collisionData.normal * (newSpeedB - speedB);

        m_Velocity = velA;
        rigidBodyB->SetVelocity( velB );

        // if other will also collide with this, mark the collision as already resolved
        if ( other->GetCollisionLayerFlags() & (1 << m_Collider->GetCollisionLayerId()) )
        {
            m_CollisionResolved = true;
        }
    }

    /// @brief Used by the Debug System to display information about this Component
    void RigidBody::Inspector()
    {
        ImGui::DragFloat2("Velocity", &m_Velocity.x);
        ImGui::DragFloat2("Acceleration", &m_Acceleration.x);
        ImGui::DragFloat("Rotational Velocity", &m_RotationalVelocity);
        ImGui::DragFloat("Mass", &m_Mass, 0.05f, 0.05f, 1000000.0f);
        ImGui::DragFloat("Restitution", &m_Restitution, 0.05f, 0.0f, 1.0f);
        ImGui::DragFloat("Friction", &m_Friction, 0.05f, 0.0f, 1000000.0f);
        ImGui::DragFloat("Drag", &m_Drag, 0.05f, 0.0f, 1000000.0f);
    }

//-----------------------------------------------------------------------------
// private: reading
//-----------------------------------------------------------------------------

    /// @brief reads the velocity from json
    /// @param data the json data
    void RigidBody::readVelocity( nlohmann::ordered_json const& data )
    {
	    m_Velocity = Stream::Read< 2, float >(data);
    }

    /// @brief reads the acceleration from json
    /// @param data the json data
    void RigidBody::readAcceleration( nlohmann::ordered_json const& data )
    {
	    m_Acceleration = Stream::Read< 2, float >(data);
    }

    /// @brief reads the rotationalVelocity from json
    /// @param data the json data
    void RigidBody::readRotationalVelocity( nlohmann::ordered_json const& data )
    {
	    m_RotationalVelocity = Stream::Read< float >(data);
    }

    /// @brief reads the inverseMass from json
    /// @param data the json data
    void RigidBody::readMass( nlohmann::ordered_json const& data )
    {
        m_Mass = Stream::Read<float>(data);
    }

    /// @brief reads the restitution from json
    /// @param data the json data
    void RigidBody::readRestitution( nlohmann::ordered_json const& data )
    {
        m_Restitution = Stream::Read<float>(data);
    }

    /// @brief reads the friction from json
    /// @param data the json data
    void RigidBody::readFriction( nlohmann::ordered_json const& data )
    {
        m_Friction = Stream::Read<float>(data);
    }

    /// @brief  reads te drag from json
    /// @param  data    the json data
    void RigidBody::readDrag( nlohmann::ordered_json const& data )
    {
        m_Drag = Stream::Read<float>(data);
    }

    /// @brief  Write all RigidBody component data to a JSON file.
    /// @return The JSON file containing the RigidBody component data.
    nlohmann::ordered_json RigidBody::Write() const
    {
        nlohmann::ordered_json data;

        data["Velocity"] = Stream::Write(m_Velocity);
        data["Acceleration"] = Stream::Write(m_Acceleration);
        data["RotationalVelocity"] = m_RotationalVelocity;
        data["InverseMass"] = m_Mass;
        data["Restitution"] = m_Restitution;
        data["Friction"] = m_Friction;
        data["Drag"] = m_Drag;

        return data;
    }

    /// @brief the map of read methods for RigidBodys
    ReadMethodMap< RigidBody > RigidBody::s_ReadMethods = {
	    { "Velocity"            , &readVelocity             },
	    { "Acceleration"        , &readAcceleration         },
	    { "RotationalVelocity"  , &readRotationalVelocity   },
        { "InverseMass"         , &readMass                 },
        { "Restitution"         , &readRestitution          },
        { "Friction"            , &readFriction             },
        { "Drag"                , &readDrag                 }
    };

//-----------------------------------------------------------------------------
// private: copying
//-----------------------------------------------------------------------------

    /// @brief  clones this RigidBody
    /// @return the newly created clone of this RigidBody
    Component* RigidBody::Clone() const
    {
        return (Component*) new RigidBody(*this);
    }

    /// @brief  copy-constructor for the RigidBody
    /// @param  other   the other RigidBody to copy
    RigidBody::RigidBody(const RigidBody& other) :
        Behavior( other ),
        m_Velocity(           other.m_Velocity           ),
        m_Acceleration(       other.m_Acceleration       ),
        m_RotationalVelocity( other.m_RotationalVelocity ),
        m_Mass(               other.m_Mass               ),
        m_Restitution(        other.m_Restitution        ),
        m_Friction(           other.m_Friction           ),
        m_CollisionResolved( false ),
        m_OnCollisionCallbackHandle( 0 ),
        m_Transform( nullptr )
    {}

//-----------------------------------------------------------------------------
