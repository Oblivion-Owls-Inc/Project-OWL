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

static float gravity = 9.81f; // Adjust this value as needed

RigidBody::RigidBody() : 
    Behavior( typeid( RigidBody ) ),
	m_Velocity(vec3(1, 1, 1)),
	m_Acceleration(vec3(1, 1, 0)),
	m_OldTranslation(vec3(0, 0, 0)),
	m_RotationalVelocity(0),
    m_InverseMass( 1.0f ),
    m_Restitution( 1.0f ),
    m_Friction( 0.0f ),
    m_CollisionResolved( false )
{
	BehaviorSystem<RigidBody>::GetInstance()->AddBehavior(this);
}

RigidBody::RigidBody(const RigidBody& other) : Behavior(typeid(RigidBody))
{
	m_RotationalVelocity = other.m_RotationalVelocity;
	m_Acceleration = other.m_Acceleration;
	m_OldTranslation = other.m_OldTranslation;
	m_Velocity = other.m_Velocity;
}

RigidBody::~RigidBody()
{
	BehaviorSystem<RigidBody>::GetInstance()->RemoveBehavior(this);
}

void RigidBody::OnUpdate(float dt)
{
	//#ifndef NDEBUG
		ImGui::Begin("Gravity");
		ImGui::InputFloat("Gravity", &gravity);
		ImGui::End();
	//#endif // !NDEBUG

}

void RigidBody::OnFixedUpdate()
{
	float dt = Engine::GetInstance()->GetFixedFrameDuration();
	Collider* collider = GetParent()->GetComponent<Collider>();
	vec3 temptranslation(0);
	Transform* m_Transform = (Transform*)GetParent()->GetComponent<Transform>();

	m_OldTranslation = m_Transform->GetTranslation();
	temptranslation = m_Transform->GetTranslation();

	// Gravity
	m_Velocity.y -= gravity * dt;


	if (m_Velocity.x > 45 || m_Velocity.y > 45)
	{
		m_Velocity.x = 45;
		m_Velocity.y = 45;
	}

	temptranslation += m_Velocity * dt;
	temptranslation.z = 0;

	float rotation = m_Transform->GetRotation();
	rotation += m_RotationalVelocity;

	m_Transform->SetRotation(rotation);
	m_Transform->SetTranslation(temptranslation);
}

Component* RigidBody::Clone() const
{
	return (Component*) new RigidBody(*this);
}

/// @brief  Called whenever a Collider on this Behavior's Entity collides
/// @param  other           the entity that was collided with
/// @param  collisionData   additional data about the collision
void RigidBody::OnCollision( Entity* other, CollisionData const& collisionData )
{
	DebugConsole output(*DebugSystem::GetInstance());
	output << GetParent()->GetName().c_str() << ":Collision Detected in RigidBody" << "\n";

    RigidBody* rigidBodyB = other->GetComponent<RigidBody>();
    if ( rigidBodyB == nullptr )
    {
        return;
    }
    if ( rigidBodyB->GetCollisionResolved() )
    {
        rigidBodyB->SetCollisionResolved( false );
        return;
    }

    Transform* transformA = GetParent()->GetComponent<Transform>();
    Transform* transformB = other->GetComponent<Transform>();

    glm::vec2 position = transformA->GetTranslation();
    position += collisionData.normal * collisionData.depth * 0.5f;
    transformA->SetTranslation(
        glm::vec3(position.x, position.y, 1.0f)
    );

    position = transformB->GetTranslation();
    position -= collisionData.normal * collisionData.depth * 0.5f;
    transformB->SetTranslation(
        glm::vec3(position.x, position.y, 1.0f)
    );

    float massA = 1.0f / m_InverseMass;
    float massB = 1.0f / rigidBodyB->GetInverseMass();

    glm::vec2 velA = m_Velocity;
    glm::vec2 velB = rigidBodyB->GetVelocity();
    float speedA = glm::dot( velA, collisionData.normal );
    float speedB = glm::dot( velB, collisionData.normal );
    float momentum = speedA * massA + speedB * massB;

    float energy = (
        speedA * speedA * massA +
        speedB * speedB * massB
    ) * m_Restitution * rigidBodyB->GetRestitution();

    float a = massA * massA + massA * massB;
    float b = -2.0f * momentum * massA;
    float c = momentum * momentum - massB * energy;

    float quadratic = (-b + std::sqrt( b * b - 4.0f * a * c )) / (a * a);

    velA += collisionData.normal * (quadratic - speedA);

    float speedB2 = (momentum - quadratic * massA) / massB;
    velB += collisionData.normal * (speedB2 - speedB);

    m_Velocity = { velA.x, velA.y, 0.0f };
    rigidBodyB->SetVelocity( { velB.x, velB.y, 0.0f } );

    m_CollisionResolved = true;
}

//-----------------------------------------------------------------------------
// private: reading
//-----------------------------------------------------------------------------

/// @brief reads the velocity from json
/// @param data the json data
void RigidBody::readVelocity(Stream data)
{
	m_Velocity = data.Read< glm::vec3 >();
}

/// @brief reads the acceleration from json
/// @param data the json data
void RigidBody::readAcceleration(Stream data)
{
	m_Acceleration = data.Read< glm::vec3 >();
}

/// @brief reads the rotationalVelocity from json
/// @param data the json data
void RigidBody::readRotationalVelocity(Stream data)
{
	m_RotationalVelocity = data.Read< float >();
}

/// @brief reads the inverseMass from json
/// @param data the json data
void RigidBody::readInverseMass(Stream data)
{
    m_InverseMass = data.Read<float>();
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
    { "InverseMass"         , &readInverseMass          },
    { "Restitution"         , &readRestitution          },
    { "Friction"            , &readFriction             }
};

/// @brief gets the map of read methods for this Component
/// @return the map of read methods for this Component
ReadMethodMap< Component > const& RigidBody::GetReadMethods() const
{
	return (ReadMethodMap< Component > const&)s_ReadMethods;
}