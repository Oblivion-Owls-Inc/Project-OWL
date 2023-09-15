#pragma once
#include "Component.h"
#include "basics.h"
#include "Transform.h"
#include "Entity.h"
#include "Behavior.h"

using namespace glm;


class RidgedBody: public Behavior
{
    public:
        RidgedBody();
        RidgedBody(const RidgedBody& other);
        ~RidgedBody();
        Component* Clone() const override;
        void FixedUpdate(float dt);

    public:
        vec3* getAcceleration();
        vec3* getVelocity();
        vec3* getOldTranslation();
        vec3* getTranslation();
        float getRotationalVelocity();
        void setAcceleration(const vec3* Acceleration);
        void setVelocity(const vec3* Velocity);
        void setOldTranslation(const vec3* OldTranslation);
        void setTranslation(const vec3* Translation);
        void SetRotationalVelocity(float rotational_velocity);

    private:
        vec3 _velocity;
        vec3 _acceleration;
        vec3 _translation;
        vec3 _oldTranslation;
        float _rotationalVelocity;
    
};

