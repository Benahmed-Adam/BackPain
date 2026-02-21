#ifndef __ENTITY_H__
#define __ENTITY_H__

#include <raylib.h>

#include <string>

class Entity {
public:
    Entity(const std::string& name, Vector3 position, Texture2D& texture,
        const Vector2& scale);
    virtual ~Entity();

    virtual void draw(const Camera3D& camera) const = 0;
    virtual void update(float dt);

    Vector3 getPosition() const;
    void setPosition(const Vector3& pos);

    Vector3 getRotation() const;

    Vector2 getScale() const;
    void setScale(const Vector2& scl);

    Vector3 getVelocity() const;
    void setVelocity(const Vector3& vel);

    void applyForce(const Vector3& force);
    void stopMovement();

    bool isActive() const;
    void setActive(bool active);

    std::string getName() const;
    void setName(const std::string& name);

    void rotateTo(const Camera3D& camera);

    float distanceTo(const Vector3& pos) const;

    int getHealth() const;
    void setHealth(int health);

    float getRadius() const;

protected:
    Vector3 position;
    Vector3 rotation;
    Vector2 scale;
    Vector3 velocity;

    Texture2D texture;

    bool active;
    int health;

private:
    std::string name;
};

#endif