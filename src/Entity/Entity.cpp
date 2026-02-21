#include "Entity.h"

#include <raymath.h>

#include "../utils/utils.h"

Entity::Entity(const std::string& name, Vector3 position, Texture2D& texture,
    const Vector2& scale)
{
    this->position = position;
    this->rotation = { 0.0f, 0.0f, 0.0f };
    this->scale = scale;
    this->velocity = { 0.0f, 0.0f, 0.0f };
    this->texture = texture;
    this->name = name + generate_uuid_v4();
    this->active = true;
    this->health = 100;
}

Entity::~Entity() { }

void Entity::update(float dt)
{
    this->position
        = Vector3Add(this->position, Vector3Scale(this->velocity, dt));
}

Vector3
Entity::getPosition() const
{
    return this->position;
}

void Entity::setPosition(const Vector3& pos)
{
    this->position = pos;
}

Vector3
Entity::getRotation() const
{
    return this->rotation;
}

Vector2
Entity::getScale() const
{
    return this->scale;
}

void Entity::setScale(const Vector2& scl)
{
    this->scale = scl;
}

Vector3
Entity::getVelocity() const
{
    return this->velocity;
}

void Entity::setVelocity(const Vector3& vel)
{
    this->velocity = vel;
}

void Entity::applyForce(const Vector3& force)
{
    this->velocity.x += force.x;
    this->velocity.y += force.y;
    this->velocity.z += force.z;
}

void Entity::stopMovement()
{
    this->velocity = { 0.0f, 0.0f, 0.0f };
}

bool Entity::isActive() const
{
    return this->active;
}

void Entity::setActive(bool state)
{
    this->active = state;
}

std::string
Entity::getName() const
{
    return this->name;
}

void Entity::setName(const std::string& n)
{
    this->name = n;
}

void Entity::rotateTo(const Camera3D& camera)
{
    Vector3 direction = Vector3Subtract(camera.position, this->position);

    direction.y = 0.0f;

    direction = Vector3Normalize(direction);

    float angle = atan2f(direction.x, direction.z);

    this->rotation.y = angle * RAD2DEG;
}

float Entity::distanceTo(const Vector3& pos) const
{
    Vector3 diff = Vector3Subtract(this->position, pos);
    return Vector3Length(diff);
}

int Entity::getHealth() const
{
    return this->health;
}

void Entity::setHealth(int health)
{
    this->health = health;
}

float Entity::getRadius() const
{
    return (this->scale.x > this->scale.y ? this->scale.x : this->scale.y)
        * 0.5f;
}