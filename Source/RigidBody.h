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
        vec3* getAcceleration();
        vec3* getVelocity();
        vec3* getOldTranslation();
        float getRotationalVelocity();
        void setAcceleration(const vec3* Acceleration);
        void setVelocity(const vec3* Velocity);
        void setOldTranslation(const vec3* OldTranslation);
        void SetRotationalVelocity(float rotational_velocity);
    private:
        vec3 _velocity;
        vec3 _acceleration;
        vec3 _oldTranslation;
        float _rotationalVelocity;
    
        RigidBody(const RigidBody& other);
        virtual Component* Clone() const override;

        virtual void OnFixedUpdate() override {}
        virtual void OnUpdate( float dt ) override;
        virtual void OnCollision( Entity* other ) override;

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

