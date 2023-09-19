#pragma once
#include "Component.h"
#include "basics.h"
#include "Transform.h"
#include "Entity.h"
#include "Behavior.h"


class RigidBody : public Behavior
{
    public:
        RigidBody();
        ~RigidBody();

    public:
        glm::vec3* getAcceleration();
        glm::vec3* getVelocity();
        glm::vec3* getOldTranslation();
        glm::vec3* getTranslation();
        float getRotationalVelocity();
        void setAcceleration(const glm::vec3* Acceleration);
        void setVelocity(const glm::vec3* Velocity);
        void setOldTranslation(const glm::vec3* OldTranslation);
        void setTranslation(const glm::vec3* Translation);
        void SetRotationalVelocity(float rotational_velocity);

    private:
        glm::vec3 _velocity;
        glm::vec3 _acceleration;
        glm::vec3 _translation;
        glm::vec3 _oldTranslation;
        float _rotationalVelocity;

        RigidBody(const RigidBody& other);
        virtual Component* Clone() const override;

        virtual void OnFixedUpdate() override;

        virtual void OnUpdate( float dt ) override {}
        virtual void CollisionEvent( Entity* other );

    private: // reading

        /// @brief reads the velocity from json
        /// @param data the json data
        void ReadVelocity( Stream data );

        /// @brief reads the acceleration from json
        /// @param data the json data
        void ReadAcceleration( Stream data );

        /// @brief reads the rotationalVelocity from json
        /// @param data the json data
        void ReadRotationalVelocity( Stream data );

        /// @brief the map of read methods for RigidBodys
        static ReadMethodMap< RigidBody > readMethods;

        /// @brief gets the map of read methods for this Component
        /// @return the map of read methods for this Component
        virtual ReadMethodMap< Component > const& getReadMethods() override;
};

