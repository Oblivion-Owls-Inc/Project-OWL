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
        m_OnCollisionCallbackHandle = GetParent()->GetComponent<Collider>()->AddOnCollisionCallback(
            std::bind( &RigidBody::OnCollision, this, std::placeholders::_1, std::placeholders::_2 )
        );
        m_Transform = GetParent()->GetComponent<Transform>();
    }

    /// @brief  called when this Component's Entity is removed from the Scene
    /// @note   NOT CALLED WHEN THE SCENE IS EXITED - that should be handled by this Component's System
    void RigidBody::OnExit()
    {
        BehaviorSystem<RigidBody>::GetInstance()->RemoveBehavior(this);
        GetParent()->GetComponent<Collider>()->RemoveOnCollisionCallback( m_OnCollisionCallbackHandle );
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

        // apply movement
	    m_Transform->Set( position, rotation );
    }

    /// @brief  Called whenever a Collider on this Behavior's Entity collides
    /// @param  other           the entity that was collided with
    /// @param  collisionData   additional data about the collision
    void RigidBody::OnCollision( Entity* other, CollisionData const& collisionData )
    {

        // only handle collisions with other rigidBodies
        RigidBody* rigidBodyB = other->GetComponent<RigidBody>();
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

        Transform* transformB = other->GetComponent<Transform>();

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
        glm::vec2 velA = m_Velocity;
        glm::vec2 velB = rigidBodyB->GetVelocity();

        float speedA = glm::dot( velA, collisionData.normal );
        float speedB = glm::dot( velB, collisionData.normal );
        float momentum = speedA * massA + speedB * massB;

        float energy = ( // techically this should be 1/2 this, but that cancells out in the algebra
            speedA * speedA * massA +
            speedB * speedB * massB
        ) * m_Restitution * rigidBodyB->GetRestitution();

        // solve the quadratic formula to get the new velocities of the objects after the collision
        float a = massA * massA + massA * massB;
        float b = -2.0f * momentum * massA;
        float c = momentum * momentum - massB * energy;

        float newSpeedA = (-b + std::sqrt( b * b - 4.0f * a * c )) / (a * a);
        float newSpeedB = (momentum - newSpeedA * massA) / massB;

        // apply the new velocities in the axis of the collision normal
        velA += collisionData.normal * (newSpeedA - speedA);
        velB += collisionData.normal * (newSpeedB - speedB);

        m_Velocity = velA;
        rigidBodyB->SetVelocity( velB );

        m_CollisionResolved = true;
    }

    /// @brief Used by the Debug System to display information about this Component
    void RigidBody::Inspector()
    {
        ImGui::DragFloat2("Velocity", &m_Velocity.x);
        ImGui::DragFloat2("Acceleration", &m_Acceleration.x);
        ImGui::DragFloat("Rotational Velocity", &m_RotationalVelocity);
        ImGui::DragFloat("Mass", &m_Mass);
        ImGui::DragFloat("Restitution", &m_Restitution);
        ImGui::DragFloat("Friction", &m_Friction);
    }

//-----------------------------------------------------------------------------
// private: reading
//-----------------------------------------------------------------------------

    /// @brief reads the velocity from json
    /// @param data the json data
    void RigidBody::readVelocity(Stream data)
    {
	    m_Velocity = data.Read< glm::vec2 >();
    }

    /// @brief reads the acceleration from json
    /// @param data the json data
    void RigidBody::readAcceleration(Stream data)
    {
	    m_Acceleration = data.Read< glm::vec2 >();
    }

    /// @brief reads the rotationalVelocity from json
    /// @param data the json data
    void RigidBody::readRotationalVelocity(Stream data)
    {
	    m_RotationalVelocity = data.Read< float >();
    }

    /// @brief reads the inverseMass from json
    /// @param data the json data
    void RigidBody::readMass(Stream data)
    {
        m_Mass = data.Read<float>();
    }

    /// @brief reads the restitution from json
    /// @param data the json data
    void RigidBody::readRestitution(Stream data)
    {
        m_Restitution = data.Read<float>();
    }

    /// @brief reads the friction from json
    /// @param data the json data
    void RigidBody::readFriction(Stream data)
    {
        m_Friction = data.Read<float>();
    }

    /// @brief the map of read methods for RigidBodys
    ReadMethodMap< RigidBody > RigidBody::s_ReadMethods = {
	    { "Velocity"            , &readVelocity             },
	    { "Acceleration"        , &readAcceleration         },
	    { "RotationalVelocity"  , &readRotationalVelocity   },
        { "InverseMass"         , &readMass                 },
        { "Restitution"         , &readRestitution          },
        { "Friction"            , &readFriction             }
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
